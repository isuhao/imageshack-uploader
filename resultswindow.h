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


#ifndef RESULTSWINDOW_H
#define RESULTSWINDOW_H

#include <QtGui/QDialog>
#include <QSharedPointer>
#include <media.h>
#include "twitterwindow.h"
#include "gallerycreator.h"

namespace Ui {
    class ResultsWindow;
}

class ResultsWindow : public QDialog {
    Q_OBJECT
    Q_DISABLE_COPY(ResultsWindow)
public:
    explicit ResultsWindow(QWidget *parent = 0);
    virtual ~ResultsWindow();
    void setResults(QSharedPointer<QVector<QPair<QSharedPointer<Media>,QStringList> > >);
    void setIndex(int index);


protected:
    virtual void changeEvent(QEvent *e);

private:
    Ui::ResultsWindow *m_ui;
    QSharedPointer<QVector<QPair<QSharedPointer<Media>,QStringList> > > results;
    int current;
    TwitterWindow twitter;
    GalleryCreator gallery;
    void translate2ndTab();
    void translate1stTab();
    QString galleryLink;

private slots:
    void prev();
    void next();
    void done();
    void tweet();
    void tweetGallery();
    void createGallery();
    void galleryReceiver(QString, QString, QString, QString);
    void galleryErrorReceiver();
};

#endif // RESULTSWINDOW_H

