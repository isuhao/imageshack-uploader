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


#include <QtSingleApplication>
#include <QTranslator>
#include <QSettings>
#include <QDebug>
#include "mainwindow.h"
#include "defines.h"
#ifdef Q_OS_WIN
#include "windowsexplorerintegrator.h"
#endif

#include "httprequest.h"
#include <QVector>
#include <QPair>
#include <QSharedPointer>

int main(int argc, char **argv)
{
    QtSingleApplication a(argc, argv);

    HTTPRequest r;
    QSharedPointer<Media> m(new Media("/home/a2k/Sample.mov"));
    QVector<QPair<QString, QString> > v;
    v.append(qMakePair(QString("public"), QString("yes")));




    a.setApplicationName("ImageShack Uploader");
    a.setOrganizationName("ImageShack");
    a.setOrganizationDomain("imageshack.us");
    a.setApplicationVersion(VERSION);

    QStringList filelist;
    for (int i=1; i< argc; i++)
        filelist << QString(argv[i]);
    qDebug() << argc << argv[1];
    if ((argc==2) && (QString(argv[1]) == "-unregister"))
    {
        qDebug() << "unregistering...";
#ifdef Q_OS_WIN
        WindowsExplorerIntegrator().deintegrate();
#endif
        QSettings sets;
        sets.clear();
        return 0;
    }
    if (a.isRunning() && argc>1)
    {
        a.sendMessage(filelist.join("\r\n"), 5000);
        return 0;
    }

    MainWindow w;
    if (filelist.size()) w.commandLineAddFile(filelist.join("\r\n"));
    QObject::connect(&a,
            SIGNAL(messageReceived(const QString&)),
            &w,
            SLOT(commandLineAddFile(const QString&)));

    #ifdef Q_OS_WIN
    WindowsExplorerIntegrator().integrate();
    #endif

    w.show();

    r.putFile(m, v);
    return a.exec();
}
