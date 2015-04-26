#include "GUI/mainwindow.h"
#include "GUI/fileslistwidget.h"
#include "GUI/player.h"
#include "GUI/fileslistmodel.h"
#include "GUI/fixedheaderview.h"
#include "fileslist.h"
#include "aboutdialog.h"
#include "showlogdialog.h"

#include <QtWidgets>

#define FILES_COUNT_TEXT QString("      File Count: %1")

MainWindow::MainWindow(FilesList *filesList)
{
    this->filesList = filesList;

    player = new Player(this);
    filesListWidget = new FilesListWidget(this->filesList, this);
    filesCountLabel = new QLabel(FILES_COUNT_TEXT.arg(0), this);

    QSplitter *splitter = new QSplitter(Qt::Vertical, this);
    //splitter->setStyleSheet("QSplitter::handle {background-color: black;}");

    splitter->addWidget(initVideoFrame());
    splitter->addWidget(initControlFrame());
    splitter->setChildrenCollapsible(false);
    //splitter->setCursor(Qt::SizeVerCursor);
    splitter->setHandleWidth(10);

    initMenu();
    setCentralWidget(splitter);
    setWindowTitle("Visual Renamer");
    setWindowIcon(QIcon(":/resources/logo_small.png"));
    resize(850, 650);

    connect(filesListWidget, SIGNAL(fileSelected(int)), SLOT(fileSelected(int)));
    connect(this->filesList, SIGNAL(renamed(int,int)), SLOT(filesRenamed(int,int)));
    connect(this->filesList, &FilesList::reseted, this, [=](){fileSelected(-1); filesCountLabel->setText(FILES_COUNT_TEXT.arg(this->filesList->getSize()));});
    connect(this->filesList, &FilesList::filesAdded, this, [=](){filesCountLabel->setText(FILES_COUNT_TEXT.arg(this->filesList->getSize()));});
    connect(this->filesList, &FilesList::removed, this, [=](){fileSelected(-1); filesCountLabel->setText(FILES_COUNT_TEXT.arg(this->filesList->getSize()));});

    this->setFocus();
}

MainWindow::~MainWindow()
{

}

bool MainWindow::eventFilter(QObject *object, QEvent *event)
{
    if (event->type() == QEvent::DragEnter)
    {
        QDragEnterEvent *dragEvent = (QDragEnterEvent*)event;
        // accept only folders
        QList<QUrl> urls = dragEvent->mimeData()->urls();
        for (const QUrl url: urls)
        {
            if (url.isLocalFile())
            {
                QFileInfo info(url.toLocalFile());

                if (info.isFile())
                {
                    return false;
                }
            }
        }
        dragEvent->setDropAction(Qt::LinkAction);
        dragEvent->acceptProposedAction();

        return true;
    }
    else if (event->type() == QEvent::Drop)
    {
        QList<QUrl> urls = ((QDropEvent*)event)->mimeData()->urls();
        QStringList files;

        for (const QUrl url: urls)
        {
            if (url.isLocalFile())
            {
                QFileInfo info(url.toLocalFile());

                if (info.isFile())
                    files.append(url.toLocalFile());
                else if (info.isDir())
                    filesList->addFolder(info.absoluteFilePath());
            }
        }

        filesList->addFiles(files);

        return true;
    }

    return QMainWindow::eventFilter(object, event);
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    if (event->matches(QKeySequence::Undo))
        filesList->undo();
    else if (event->matches(QKeySequence::Redo))
        filesList->redo();
    else if(event->key() == Qt::Key_Space)
        player->togglePause();

    QMainWindow::keyPressEvent(event);
}

void MainWindow::fileSelected(int index)
{
    if (index >= 0)
    {
        const FileEntry &file = filesList->getFile(index);

        switch (file.getType())
        {
        case FileEntry::Video:
            player->open(file.getFullPath());
            break;

        case FileEntry::Audio:
            player->openAudio(file.getFullPath());
            break;

        case FileEntry::Image:
            player->openImage(file.getFullPath());
            break;

        default:
#ifdef MACOSX
            player->openImage(qApp->applicationDirPath().append("/../Resources/doc_placeholder.jpg"));
#else
            player->openImage(qApp->applicationDirPath().append("/doc_placeholder.jpg"));
#endif

            break;
        }
    }
    else
    {
#ifdef MACOSX
            player->openImage(qApp->applicationDirPath().append("/../Resources/placeholder.jpg"));
#else
            player->openImage(qApp->applicationDirPath().append("/placeholder.jpg"));
#endif
    }
}

void MainWindow::filesRenamed(int count, int successCount)
{
//    QMessageBox *messageBox = new QMessageBox(this);
//    messageBox->setWindowTitle("Visual Renamer");
//    messageBox->setIconPixmap(QPixmap(":/resources/logo.png"));
//    messageBox->addButton(QMessageBox::Ok);

    if(count == successCount)
    {
        QDialog succesDialog;

        QLabel *logoLabel = new QLabel(&succesDialog);
        QLabel *messageLabel = new QLabel("All files have been\nrenamed successfully!", &succesDialog);
        QLabel *succesLabel = new QLabel(&succesDialog);
        QPushButton *okButton = new QPushButton("Ok");

        succesDialog.setStyleSheet("QFrame#add_files_frame {"
                             "border: 3px dashed rgb(220, 220, 220);"
                             "border-radius: 24px"
                             "}"
                             "QPushButton {"
                             "background: qlineargradient(x1:0.5, y1:0, x2:0.5, y2:1, stop:0 rgb(245, 245, 245), stop:1 rgb(214, 214, 214));"
                             "border: 1px solid rgb(171, 171, 171);"
                             "border-radius: 5px"
                             "}"
                             "QPushButton:pressed {"
                             "background: qlineargradient(x1:0.5, y1:0, x2:0.5, y2:1, stop:0 rgb(214, 214, 214), stop:1 rgb(230, 230, 230));"
                             "}"
                             "QWidget {"
                             "font: bold 13px \"Arial\";"
                             "color: rgb(58, 58, 58);"
                             "}");

        logoLabel->setPixmap(QPixmap(":/resources/logo.png"));
        succesLabel->setPixmap(QPixmap(":/resources/done.png"));
        messageLabel->setMargin(10);
        succesLabel->setMargin(10);

        okButton->setFixedSize(150, 30);

        QHBoxLayout *upperLayout = new QHBoxLayout;
        QVBoxLayout *mainLayout = new QVBoxLayout(&succesDialog);

        upperLayout->addWidget(logoLabel);
        upperLayout->addWidget(messageLabel);
        upperLayout->addWidget(succesLabel);

        mainLayout->addLayout(upperLayout);
        mainLayout->addWidget(okButton, 0, Qt::AlignHCenter);
        mainLayout->setSizeConstraint(QLayout::SetFixedSize);

        connect(okButton, SIGNAL(clicked()), &succesDialog, SLOT(accept()));

        succesDialog.exec();
    }
    else
    {
        QDialog failDialog;

        QLabel *logoLabel = new QLabel(&failDialog);
        QLabel *successMessageLabel = new QLabel(QString("%1 of %2 files have been\nrenamed successfully!").arg(successCount).arg(count), &failDialog);
        QLabel *failMessageLabel = new QLabel(QString("ERROR: %1 of %2 files\nhave not been renamed.").arg(count-successCount).arg(count), &failDialog);
        QLabel *succesLabel = new QLabel(&failDialog);
        QLabel *failLabel = new QLabel(&failDialog);

        QPushButton *okButton = new QPushButton("Ok");

        failDialog.setStyleSheet("QFrame#add_files_frame {"
                             "border: 3px dashed rgb(220, 220, 220);"
                             "border-radius: 24px"
                             "}"
                             "QPushButton {"
                             "background: qlineargradient(x1:0.5, y1:0, x2:0.5, y2:1, stop:0 rgb(245, 245, 245), stop:1 rgb(214, 214, 214));"
                             "border: 1px solid rgb(171, 171, 171);"
                             "border-radius: 5px"
                             "}"
                             "QPushButton:pressed {"
                             "background: qlineargradient(x1:0.5, y1:0, x2:0.5, y2:1, stop:0 rgb(214, 214, 214), stop:1 rgb(230, 230, 230));"
                             "}"
                             "QWidget {"
                             "font: bold 13px \"Arial\";"
                             "color: rgb(58, 58, 58);"
                             "}");

        logoLabel->setPixmap(QPixmap(":/resources/logo.png"));
        succesLabel->setPixmap(QPixmap(":/resources/done.png"));
        failLabel->setPixmap(QPixmap(":/resources/fail.png"));

        okButton->setFixedSize(150, 30);

        QVBoxLayout *messagesLayout = new QVBoxLayout;
        messagesLayout->addWidget(successMessageLabel);
        messagesLayout->addWidget(failMessageLabel);
        messagesLayout->setMargin(0);
        messagesLayout->setSpacing(0);

        QVBoxLayout *imagesLayout = new QVBoxLayout;
        imagesLayout->addWidget(succesLabel);
        imagesLayout->addWidget(failLabel);
        imagesLayout->setMargin(0);
        imagesLayout->setSpacing(0);

        QHBoxLayout *statusLayout = new QHBoxLayout;
        statusLayout->addLayout(messagesLayout);
        statusLayout->addLayout(imagesLayout);

        QHBoxLayout *upperLayout = new QHBoxLayout;
        upperLayout->addWidget(logoLabel);
        upperLayout->addLayout(statusLayout);

        QVBoxLayout *mainLayout = new QVBoxLayout(&failDialog);
        mainLayout->addLayout(upperLayout);
        mainLayout->addWidget(okButton, 0, Qt::AlignHCenter);
        mainLayout->setSizeConstraint(QLayout::SetFixedSize);

        connect(okButton, SIGNAL(clicked()), &failDialog, SLOT(accept()));

        failDialog.exec();
    }
}

QFrame* MainWindow::initVideoFrame()
{
    QFrame *frame = new QFrame(this);

    QVBoxLayout *layout = new QVBoxLayout();

    player->setMinimumHeight(240);

#ifdef MACOSX
            player->openImage(qApp->applicationDirPath().append("/../Resources/placeholder.jpg"));

#else
            player->openImage(qApp->applicationDirPath().append("/placeholder.jpg"));
#endif

    frame->setObjectName("video_frame");
    frame->setStyleSheet("QFrame#video_frame { background: qlineargradient(x1:0.5, y1:0, x2:0.5, y2:1, stop:0 rgb(8, 9, 9), stop:0.6 rgb(40, 40, 40), "
                         "stop:0.8 rgb(57, 57, 57), stop:0.9 rgb(51, 51, 51), stop:1 rgb(31, 31, 31)) } ");

    frame->setLayout(layout);


    layout->setMargin(0);
    layout->addWidget(player);

    return frame;
}

QFrame* MainWindow::initControlFrame()
{
    QFrame *frame = new QFrame(this);
    QVBoxLayout *layout = new QVBoxLayout(frame);

    frame->setStyleSheet("QFrame { background-color: white } ");

    layout->setAlignment(Qt::AlignTop);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);
    layout->addWidget(initFilesFrame(frame));
    layout->addWidget(initRenameFrame(frame));
    layout->addWidget(initFilesListFrame(frame));
    layout->addWidget(initStatusBarFrame(frame));

    return frame;
}

QFrame* MainWindow::initFilesFrame(QWidget *parent)
{
    QFrame *frame = new QFrame(parent);

    //QAbstractButton *addFilesButton = createIconButton(QString(":/resources/add_files_button%1.png"), "Add Files", frame);
    QAbstractButton *addFolderButton = createIconButton(QString(":/resources/add_folder_button%1.png"), "", frame);
    QAbstractButton *resetButton = createIconButton(QString(":/resources/reset_button%1.png"), "", frame);
    QAbstractButton *renameButton = createIconButton(QString(":/resources/rename_button%1.png"), "", frame);

    QHBoxLayout *layout = new QHBoxLayout(frame);

    frame->setFixedHeight(56);

    //addFolderButton->setFixedWidth(175);
    //resetButton->setFixedWidth(125);
    //renameButton->setFixedWidth(85);

    frame->setStyleSheet("QFrame {"
                             "background: qlineargradient(x1:0.5, y1:0, x2:0.5, y2:1, stop:0 rgb(60, 60, 60), stop:1 rgb(60, 60, 60));"
                             //"border-bottom: 1px solid rgb(198, 198, 198);"
                             "image: url(:/resources/frame_center.png);"
                         "}"
                         "QAbstractButton {"
                             "background: transparent;"
                             "font: bold 13px \"Arial\";"
                             "color: rgb(58, 58, 58);"
                         "}");

    layout->setContentsMargins(0, 2, 0, 2);
    layout->setSpacing(90);
    layout->setAlignment(Qt::AlignCenter);
    //layout->addWidget(addFilesButton);
    layout->addWidget(addFolderButton);
    layout->addWidget(resetButton);
    layout->addWidget(renameButton);

    //connect(addFilesButton, &QPushButton::clicked, this, [=](){ filesList->addFiles(QFileDialog::getOpenFileNames(this, "Select Files")); });
    connect(addFolderButton, &QPushButton::clicked, this, [=](){ filesList->addFolder(QFileDialog::getExistingDirectory(this, "Select Directory")); });
    connect(resetButton, SIGNAL(clicked()), filesList, SLOT(reset()));
    connect(renameButton, SIGNAL(clicked()), filesList, SLOT(renameFiles()));

    return frame;
}

/*void MainWindow::initSettingsFrame()
{
    QDialog settingsDialog;

    QLabel *settingsLabel = new QLabel(&settingsDialog);
    QLabel *settingsMessageLabel = new QLabel("Please enter the Number of Digits\nfor increments", &settingsDialog);
    QLabel *digitsLabel = new QLabel("Digits:", &settingsDialog);
    QLabel *previewLabel = new QLabel("Preview:", &settingsDialog);

    QLineEdit *editLine = new QLineEdit(&settingsDialog);
    QLineEdit *previewLine = new QLineEdit(&settingsDialog);

    QPushButton *okButton = new QPushButton("OK");

    editLine->setFixedWidth(200);
    editLine->setValidator(new QIntValidator(1, 25, this));

    previewLine->setFixedWidth(200);
    previewLine->setEnabled(false);

    settingsDialog.setStyleSheet("QFrame#add_files_frame {"
                         "border: 3px dashed rgb(220, 220, 220);"
                         "border-radius: 24px"
                         "}"
                         "QPushButton {"
                         "background: qlineargradient(x1:0.5, y1:0, x2:0.5, y2:1, stop:0 rgb(245, 245, 245), stop:1 rgb(214, 214, 214));"
                         "border: 1px solid rgb(171, 171, 171);"
                         "border-radius: 5px"
                         "}"
                         "QPushButton:pressed {"
                         "background: qlineargradient(x1:0.5, y1:0, x2:0.5, y2:1, stop:0 rgb(214, 214, 214), stop:1 rgb(230, 230, 230));"
                         "}"
                         "QWidget {"
                         "font: bold 13px \"Arial\";"
                         "color: rgb(58, 58, 58);"
                         "}");

    settingsLabel->setPixmap(QPixmap(":/resources/warning.png"));

    okButton->setFixedSize(150, 30);

    QHBoxLayout *hintLayout = new QHBoxLayout;
    hintLayout->addWidget(settingsLabel);
    hintLayout->addWidget(settingsMessageLabel);

    QHBoxLayout *textLineLayout = new QHBoxLayout;
    textLineLayout->addWidget(digitsLabel, 0, Qt::AlignRight);
    textLineLayout->addWidget(editLine, 0, Qt::AlignLeft);

    QHBoxLayout *previewLayout = new QHBoxLayout;
    previewLayout->addWidget(previewLabel, 0, Qt::AlignRight);
    previewLayout->addWidget(previewLine, 0, Qt::AlignLeft);

    QVBoxLayout *mainLayout = new QVBoxLayout(&settingsDialog);
    mainLayout->addLayout(hintLayout);
    mainLayout->addLayout(textLineLayout);
    mainLayout->addLayout(previewLayout);
    mainLayout->addWidget(okButton, 0, Qt::AlignCenter);
    mainLayout->setMargin(20);
    mainLayout->setSizeConstraint(QLayout::SetFixedSize);

    connect(okButton,  &QPushButton::clicked, this, [=]()
    {
        if (!editLine->text().isEmpty() && editLine->text() != "0")
            qApp->setProperty("NUMBER_OF_ZEROS", editLine->text().prepend("%0").append('d'));
        else
            qApp->setProperty("NUMBER_OF_ZEROS", "");

        filesList->save();
//        filesList->reset();
        filesList->load();
    });
    connect(editLine, &QLineEdit::textChanged, this, [=](const QString &string)
    {
        if (!string.isEmpty() && string != "0")
        {
            QString text = string;
            text.sprintf(text.prepend("%0").append('d').toLatin1(), 1);

            previewLine->setText(text);
        }
        else
            previewLine->clear();
    });
    connect(okButton, SIGNAL(clicked()), &settingsDialog, SLOT(accept()));

    settingsDialog.exec();
}*/

QFrame* MainWindow::initRenameFrame(QWidget *parent)
{
    QFrame *frame = new QFrame(parent);

    QAbstractButton *copyDownButton = createIconButton(QString(":/resources/copy_down_button%1.png"), "Copy Down", frame);
    QCheckBox *autoRenameBox = new QCheckBox("Auto Rename", frame);

    QHBoxLayout *layout = new QHBoxLayout(frame);

    frame->setFixedHeight(36);
    frame->setStyleSheet("QFrame {"
                             "background: qlineargradient(x1:0.5, y1:0, x2:0.5, y2:0.6, x3:0.5, y3:1,"
                                                          "stop:0 rgb(245, 245, 245),"
                                                          "stop:0.6 rgb(236, 236, 236),"
                                                          "stop:1 rgb(220, 220, 220));"
                             "border-bottom: 1px solid rgb(185, 185, 185);"
                         "}"
                         "QCheckBox::indicator:unchecked { image: url(:/resources/check_box_unchecked.png); }"
                         "QCheckBox::indicator:unchecked:pressed { image: url(:/resources/check_box_unchecked_pressed.png); }"
                         "QCheckBox::indicator:checked { image: url(:/resources/check_box_checked.png); }"
                         "QCheckBox::indicator:checked:pressed { image: url(:/resources/check_box_checked_pressed.png); }"
                         "QAbstractButton {"
                             "background: transparent;"
                             "font: bold 11px \"Arial\";"
                             "color: rgb(58, 58, 58);"
                         "}");

    autoRenameBox->setCursor(Qt::PointingHandCursor);

    layout->setContentsMargins(0, 2, 0, 2);
    layout->setSpacing(20);
    layout->setAlignment(Qt::AlignCenter);
    layout->addWidget(copyDownButton);
    layout->addWidget(autoRenameBox);

    connect(copyDownButton, SIGNAL(clicked()), filesListWidget, SLOT(copyDown()));
    connect(autoRenameBox, SIGNAL(toggled(bool)), filesList, SLOT(setAutoRename(bool)));

    return frame;
}

QFrame* MainWindow::initFilesListFrame(QWidget *parent)
{
    QScrollArea *scrollFrame = new QScrollArea(parent);
    QFrame *frame = new QFrame(scrollFrame);

    QVBoxLayout *layout = new QVBoxLayout(frame);

    layout->setContentsMargins(0, 0, 0, 0);
    layout->setAlignment(Qt::AlignTop);
    layout->addWidget(filesListWidget);
    layout->addSpacing(25);
    layout->addWidget(initDragFrame(frame), 0, Qt::AlignHCenter);
    layout->addSpacing(25);

    scrollFrame->setWidget(frame);
    scrollFrame->setAcceptDrops(true);
    scrollFrame->installEventFilter(this);
    scrollFrame->setWidgetResizable(true);
    scrollFrame->setStyleSheet("QFrame {"
                               "border: none;"
                               "}"
                               "QScrollBar:vertical {"
                               "background: white;"
                               "width: 8px;"
                               "}"
                               "QScrollBar::handle:vertical {"
                               "background: grey;"
                               "min-height: 20px;"
                               "}"
                               "QScrollBar::add-line:vertical, QScrollBar::sub-line:vertical {"
                               "width: 0px;"
                               "height: 0px;"
                               "}");

    QFrame *mainFrame = new QFrame(parent);
    QVBoxLayout *mainLayout = new QVBoxLayout(mainFrame);
    FixedHeaderView *headerView = new FixedHeaderView(Qt::Horizontal, mainFrame);
    headerView->setModel(new FilesListModel(filesList, this));
    headerView->setStyleSheet("QHeaderView::section {"
                              "background: transparent;"
                              "border-style: none;"
                              "border-bottom: 1px solid rgb(207, 207, 207);"
                              "border-right: 1px solid rgb(207, 207, 207);"
                              "}"
                              "QWidget {"
                              "font: bold 11px \"Arial\";"
                              "color: rgb(58, 58, 58);"
                              "}");
    headerView->setFixedHeight(30);
    headerView->setMinimumSectionSize(150);
    headerView->resizeSection(0, 250);
    headerView->resizeSection(1, 400);
    headerView->setFixedHeight(30);

    mainLayout->setMargin(0);
    mainLayout->setSpacing(0);
    mainLayout->addWidget(headerView);
    mainLayout->addWidget(scrollFrame);

    connect(headerView, SIGNAL(sectionResized(int,int,int)), filesListWidget, SLOT(resizeSection(int, int, int)));
    connect(headerView, SIGNAL(settingsButtonClicked()), SIGNAL(incrementSettingsClicked()));
    //connect(headerView, &FixedHeaderView::settingsButtonClicked, this, [=](){ initSettingsFrame(); });

    connect(filesListWidget, &FilesListWidget::fileSelected, this, [=](int index)
    {
        if (index >= 0)
        {
            QRect rect = filesListWidget->visualRect(filesListWidget->currentIndex());

            scrollFrame->ensureVisible(rect.x(), rect.y() + rect.height(), 0, rect.height());
        }
    });

    return mainFrame;
}

QFrame* MainWindow::initStatusBarFrame(QWidget *parent)
{
    QFrame *frame = new QFrame(parent);
    QHBoxLayout *mainLayout = new QHBoxLayout(frame);

    filesCountLabel->setStyleSheet("QLabel {"
                                   "font: bold 12px \"Arial\";"
                                   "color: rgb(123, 123, 123);"
                                   "border: none;"
                                   "background: transparent;"
                                   "image: url(:/resources/files_count.png);"
                                   "image-position: left;"
                                   "}");

    frame->setFixedHeight(30);
    frame->setStyleSheet("QFrame {"
                         "background: qlineargradient(x1:0.5, y1:0, x2:0.5, y2:1,"
                         "stop:0 rgb(245, 245, 245),"
                         "stop:1 rgb(205, 205, 205));"
                         "border-top: 1px solid rgb(217, 217, 217);"
                         "}");

    mainLayout->setAlignment(Qt::AlignCenter);
    mainLayout->setMargin(0);
    mainLayout->addWidget(filesCountLabel);

    return frame;
}

QFrame* MainWindow::initDragFrame(QWidget *parent)
{
    QFrame *frame = new QFrame(parent);

    QLabel *logoLabel = new QLabel(frame);
    QLabel *dragLabel = new QLabel("Drop Folders Here Or", frame);
    //QPushButton *addFilesButton = new QPushButton("Select files", frame);
    QPushButton *addFilesButton = new QPushButton("Select Folder", frame);

    logoLabel->setPixmap(QPixmap(":/resources/logo.png"));
    dragLabel->setAlignment(Qt::AlignCenter);

    QHBoxLayout *mainLayout = new QHBoxLayout(frame);
    QVBoxLayout *addFilesLayout = new QVBoxLayout();

    addFilesButton->setFixedSize(290, 30);

    addFilesLayout->setAlignment(Qt::AlignCenter);
    addFilesLayout->addWidget(dragLabel);
    addFilesLayout->addWidget(addFilesButton);

    mainLayout->addSpacing(25);
    mainLayout->addWidget(logoLabel);
    mainLayout->addLayout(addFilesLayout);
    mainLayout->addSpacing(65);

    frame->setFixedSize(570, 155);
    frame->setObjectName("add_files_frame");
    frame->setStyleSheet("QFrame#add_files_frame {"
                         "border: 3px dashed rgb(220, 220, 220);"
                         "border-radius: 24px"
                         "}"
                         "QPushButton {"
                         "background: qlineargradient(x1:0.5, y1:0, x2:0.5, y2:1, stop:0 rgb(245, 245, 245), stop:1 rgb(214, 214, 214));"
                         "border: 1px solid rgb(171, 171, 171);"
                         "border-radius: 5px"
                         "}"
                         "QPushButton:pressed {"
                         "background: qlineargradient(x1:0.5, y1:0, x2:0.5, y2:1, stop:0 rgb(214, 214, 214), stop:1 rgb(230, 230, 230));"
                         "}"
                         "QWidget {"
                         "font: bold 13px \"Arial\";"
                         "color: rgb(58, 58, 58);"
                         "}");

    setMinimumWidth(frame->width()+10);

    //connect(addFilesButton, &QPushButton::clicked, this, [=](){ filesList->addFiles(QFileDialog::getOpenFileNames(this, "Select Files")); });
    connect(addFilesButton, &QPushButton::clicked, this, [=](){ filesList->addFolder(QFileDialog::getExistingDirectory(this, "Select Directory")); });


    return frame;
}

void MainWindow::initMenu()
{
    QMenu *fileMenu = new QMenu("File", this);

    QAction *saveAction = new QAction("Save", fileMenu);
    QAction *loadAction = new QAction("Load", fileMenu);
    QAction *playAction = new QAction("Play", fileMenu);
    //QAction *logAction = new QAction("Show log", fileMenu);


    saveAction->setShortcut(QKeySequence::Save);
    loadAction->setShortcut(QKeySequence("Ctrl+L"));
    playAction->setShortcut(QKeySequence("Space"));

    fileMenu->addAction(saveAction);
    fileMenu->addAction(loadAction);
    fileMenu->addAction(playAction);
    //fileMenu->addAction(logAction);

    menuBar()->addMenu(fileMenu);

    connect(saveAction, SIGNAL(triggered()), filesList, SLOT(save()));
    connect(loadAction, SIGNAL(triggered()), filesList, SLOT(load()));
    connect(playAction, SIGNAL(triggered()), player, SLOT(togglePause()));
    //connect(logAction, SIGNAL(triggered()), SLOT(showLog()));

    // add "About" item
    QMenu *helpMenu = new QMenu(tr("&Help"), this);
    QAction *aboutAction = helpMenu->addAction(tr("&About"));
    connect(aboutAction, SIGNAL(triggered()), this, SLOT(showAbout()));
    menuBar()->addMenu(helpMenu);
}

void MainWindow::showLog()
{
    ShowLogDialog showLogDialog(this);
    showLogDialog.exec();
}


void MainWindow::showAbout()
{
    AboutDialog aboutDialog(this);
    aboutDialog.exec();
}


QAbstractButton* MainWindow::createIconButton(const QString &iconPath, const QString &text, QWidget *parent) const
{
    QCheckBox *button = new QCheckBox(text, parent);

    //button->setIcon(QIcon(iconPath));
    button->setStyleSheet("QCheckBox::indicator { image: url(" + iconPath.arg("") + "); }"
                          "QCheckBox::indicator:hover { image: url(" + iconPath.arg("_hovered") + "); }"
                          "QCheckBox::indicator:pressed { image: url(" + iconPath.arg("_pressed") + "); }");
    button->setIconSize(QImage(iconPath).size());
    button->setContentsMargins(0, 0, 0, 0);
    button->setCursor(Qt::PointingHandCursor);
    button->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

    return button;
}
