/*
Copyright (c) 2009, ImageShack Corp.
All rights reserved.

Redistribution and use in source and binary forms, with or without modification,
are permitted provided that the following conditions are met:
* Redistributions of source code must retain the above copyright notice, this
  list of conditions and the following disclaimer.
* Redistributions in binary form must reproduce the above copyright notice,
  this list of conditions and the following disclaimer in the documentation
  and/or other materials provided with the distribution.
* Neither the name of the ImageShack nor the names of its contributors may be
  used to endorse or promote products derived from this software without
  specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/


#include "fileuploader.h"
#include <QDebug>
#include <QDomElement>
#include <QMessageBox>

FileUploader::FileUploader(ProgressWidget *prog, MediaListModel* list)
{
    progress = prog;
    medias = list;
    donecount = 0;
    filecount = list->rowCount();
    failed = false;
    //http = QSharedPointer<QHttp>(new QHttp);
    res = QSharedPointer<QVector<QPair<QSharedPointer<Media>,QStringList> > >
          ( new QVector<QPair<QSharedPointer<Media>,QStringList> >);
    connect(&seconds, SIGNAL(timeout()),
            this, SLOT(updateETA()));
    uploadedTotal = 0;
    uploadedCurrent = 0;
}

void FileUploader::begin()
{
    emit status(0);
    progress->setProgress(0,0);
    progress->setCurrentIndex(1);
    failcount = 0;
    skip = 0;
    totalsize = medias->totalSize();
    process();
}

void FileUploader::process()
{
    uploadedCurrent = 0;
    medias->skipped = this->skip;
    if (failed) { failed = true; emit status(2); return;}
    if (medias->rowCount() && (skip < medias->rowCount()))
    {
        if (!current.isNull() && !request.isNull() && !request.data()->aborted && !request.data()->failed)
            uploadedTotal += current.data()->size();
        current = medias->getMedia(skip);
        if (!QFile::exists(current.data()->filepath()))
        {
            qDebug() << current.data()->filename() << "does not exist";
            fail(tr("File does not exist"));
            return process();
        }
        request = QSharedPointer<HTTPRequest>(new HTTPRequest());
        tmp.append(request);
        request.data()->connectProgress(this, SLOT(progressReceiver(int)));
        request.data()->connectError(this, SLOT(uploadErrorReceiver(QString)));
        request.data()->connectResult(this, SLOT(resultReceiver(QString)));

        seconds.start(1000);
        request.data()->uploadFile(current, cookie);
    }
    else
    {
        seconds.stop();
        progress->setCurrentIndex(0);
        emit status(1);
        if (skip)
        {
            QString text = tr("Some files could not be uploaded.");
            if (errors.size())
            {
                text.append(tr("\n\nError messages:\n", 0, errors.size()));
                text.append(errors.join("\n"));
            }
            QMessageBox::critical(progress, tr("Warning"), text);
        }
        if (!(res.data()->isEmpty())) emit results(res);
    }
}

void FileUploader::progressReceiver(int value)
{
    int total = (int)(((100.0/filecount)*donecount)+((100.0/filecount)/100*value));
    progress->setProgress(total, value);
    if (value == 100) seconds.stop();
}

void FileUploader::statusReceiver(int value)
{
    if (value == 2)
    {
        failcount++;
        if (failcount>3)
        {
            fail(tr("3 attempts to upload file failed"));
        }
        process();
    }
}

void FileUploader::uploadErrorReceiver(QString msg)
{
    failcount++;
    errors.append(current.data()->filename() + ":" + msg);
    if (failcount>3) fail(tr("3 attempts to upload file failed"));
    process();
}

void FileUploader::resultReceiver(QString value)
{
    qDebug() << value;
    seconds.stop();
    QDomDocument xml;
    xml.setContent(value);
    QDomElement doc = xml.documentElement();
    if (doc.isNull())
    {
        fail("Server did not return valid response");
        process();
        return;
    }
    QDomElement error = doc.firstChildElement("error");
    if (!error.isNull())
    {
        if (!error.text().isEmpty())
            fail(error.text());
        else fail(tr("Server returned no error text"));
        process();
        return;
    }
    QDomElement links = doc.firstChildElement("links");
    if (links.isNull())
    {
        fail(tr("Wrong server response"));
        process();
        return;
    }

    QStringList all;
    QDomElement image_link, image_html, image_bb, image_bb2, yfrog_link,
                yfrog_thumb, thumb_html, thumb_bb, thumb_bb2, ad_link;

    image_link = links.firstChildElement("image_link");
    if (current.data()->getClass() == "video")
    {
        image_html = links.firstChildElement("frame_html");
        image_bb = links.firstChildElement("frame_bb");
        image_bb2 = links.firstChildElement("frame_bb2");
    }
    else
    {
        image_html = links.firstChildElement("image_html");
        image_bb = links.firstChildElement("image_bb");
        image_bb2 = links.firstChildElement("image_bb2");
    }
    yfrog_link = links.firstChildElement("yfrog_link");
    yfrog_thumb = links.firstChildElement("yfrog_thumb");
    thumb_html = links.firstChildElement("thumb_html");
    thumb_bb = links.firstChildElement("thumb_bb");
    thumb_bb2 = links.firstChildElement("thumb_bb2");
    ad_link = links.firstChildElement("ad_link");
    if (image_link.isNull() || image_html.isNull() || image_bb.isNull() ||
        image_bb2.isNull() || yfrog_link.isNull() || yfrog_thumb.isNull() ||
        thumb_html.isNull() || thumb_bb.isNull() || thumb_bb2.isNull() ||
        ad_link.isNull())
    {
        fail(tr("Server responce is not valid"));
        process();
        return;
    }
    all << image_link.text() << image_html.text() << image_bb.text()
            << image_bb2.text() << yfrog_link.text() << yfrog_thumb.text()
            << thumb_html.text() << thumb_bb.text() << thumb_bb2.text()
            << ad_link.text();

    failcount = 0;
    donecount += 1;
    res.data()->append(qMakePair(current, all));
    medias->removeMedia(skip);
    if (!failed) process();
}

void FileUploader::setCookie(QString value)
{
    cookie = value;
}

void FileUploader::cancel()
{
    if (NULL != request.data())
    {
        request->blockSignals(true);
        request->stop();

        int index = tmp.indexOf(request);
        tmp.remove(index);
        request.clear();
    }

    seconds.stop();
    emit status(3);
    progress->setCurrentIndex(0);
}

void FileUploader::fail(QString message)
{
    qDebug() << "FileUploader failing";
    QString filename = medias->getMedia(skip).data()->filename();
    if (!message.isNull())
        message = filename + ": " + message;
    if (message.size())
    {
        if (!errors.contains(message))
            errors.append(message);
    }
    else
    {
        QString errorString = request.data()->errorString();
        if (errorString != "Unknown error")
                errors.append(filename + ": " + errorString);
        else
        {
            errors.append(filename + ": " + tr("Could not connect to server"));
        }
    }
    skip++;
}

void FileUploader::updateETA()
{
    qint64 uploadedPrevious = uploadedCurrent;
    uploadedCurrent = request.data()->uploaded;
    qint64 speed = uploadedCurrent - uploadedPrevious;
    qint64 totalUploadedSize = uploadedTotal + uploadedCurrent;

    if (totalsize < totalUploadedSize) return;
    if (speed <= 0) return;
    int timeLeft = (totalsize - totalUploadedSize)/speed;

    qDebug() << "uploadedTotal:" << uploadedTotal;
    qDebug() << "request.data()->uploaded:" << request.data()->uploaded;
    qDebug() << "request.data()->doneSize:" << request.data()->doneSize;
    qDebug() << "uploaded:" << totalUploadedSize;
    qDebug() << "totalsize:" << totalsize;
    qDebug() << "speed:" << speed;
    qDebug() << "left" << timeLeft;
    emit ETA(timeLeft);
}

void FileUploader::pause(bool st)
{
    if (st)
    {
        if (NULL != request.data())
        {
            request->blockSignals(true);
            request->stop();

            int index = tmp.indexOf(request);
            tmp.remove(index);
            request.clear();
        }

        seconds.stop();
        int total = (int)((100.0/filecount)*donecount);

        emit ETA(-100);
        uploadedTotal = 0;
        if (current.data()->getClass() != "video")
        {
            emit ETA(-100);
            progress->setProgress(total, 0);
        }
    }
    else
    {
        process();
    }
}
