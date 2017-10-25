#include "mainwindow.h"
#include "ui_mainwindow.h"



MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->centralWidget->layout()->setMargin(0);
    ui->pushButton_Undo->setEnabled(false);
    ui->actionUndo->setEnabled(false);
    ui->pushButton_Redo->setEnabled(false);
    ui->actionRedo->setEnabled(false);
    ui->pushButton_Copy->setEnabled(false);
    ui->actionCopy->setEnabled(false);
    ui->pushButton_Cut->setEnabled(false);
    ui->actionCut->setEnabled(false);
    ui->pushButton_Paste->setEnabled(true);
    ui->actionPaste->setEnabled(true);
    QClipboard *buf = QApplication::clipboard();
    if(buf->text().isEmpty())
    {
        ui->pushButton_Paste->setEnabled(false);
        ui->actionPaste->setEnabled(false);
    }
    QSettings s(QCoreApplication::applicationDirPath()+QDir::separator()+"settings.ini",QSettings::IniFormat);
    this->Font=qvariant_cast<QFont>(s.value("Font",this->Font));
    ui->textEdit->setFont(this->Font);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_Create_clicked()
{
    MainWindow *w = new MainWindow();
    if(this->next!=NULL)
    {
        this->next->prev=w;
        w->next=this->next;
        w->prev=this;
        this->next=w;
    }
    else
    {
        w->prev=this;
        this->next=w;
    }
    w->show();
}

void MainWindow::on_pushButton_Open_clicked()
{
    if(!this->isSaved && this->isChanged)
    {
        QMessageBox::StandardButton que;
        que = QMessageBox::question(this, this->Name+QString::fromUtf8(" - MyTextEditor"),
                                    QString::fromUtf8("Do you want to save file?"),
                                    QMessageBox::Yes |QMessageBox::No | QMessageBox::Cancel,QMessageBox::No);
        if(que==QMessageBox::Yes)
        {
            on_pushButton_Save_clicked();
        }
        if(que==QMessageBox::Cancel)
        {
            return;
        }

    }
    QString filename = QFileDialog::getOpenFileName(this, tr("Open file"), QDir::currentPath(), tr("files (*.txt)"));
    if(filename!="")
    {
        QFile file(filename);
        if(!file.open(QIODevice::ReadOnly))
        {
            QMessageBox::critical(this, tr("Error"), tr("Can't open file"));
            return;
        }
        QTextStream in(&file);
        ui->textEdit->setText(in.readAll());
        this->Path=filename;
        this->isSaved=true;
        this->isChanged=false;
        this->setWindowTitle(filename.split("/")[filename.split("/").length()-1]+" - MyTextEditor");
    }

}

void MainWindow::on_pushButton_Save_clicked()
{
    if(this->isChanged)
    {
        if(this->Path.isEmpty())
        {
            this->Path = QFileDialog::getSaveFileName(0, "Save file", "", "*.txt");
        }
        this->Name = this->Path.split("/")[this->Path.split("/").length()-1];
        if(this->Path.isEmpty())
        {
            return;
        }
        this->setWindowTitle(this->Name+" - MyTextEditor");
        QFile file(this->Path);
        if(!file.open(QIODevice::WriteOnly | QIODevice::Text))
        {
            qFatal("Couldn't open file ");
        }
        QTextStream ts (&file);
        ts<<ui->textEdit->toPlainText();
        file.close();
        this->isSaved=true;
        this->isChanged=false;
    }
}

void MainWindow::on_pushButton_Close_clicked()
{
    this->close();
}

void MainWindow::on_pushButton_Copy_clicked()
{
    ui->textEdit->copy();
    ui->pushButton_Copy->setEnabled(false);
    ui->pushButton_Cut->setEnabled(false);
    ui->pushButton_Paste->setEnabled(true);
    qDebug()<<ui->pushButton_Paste->isEnabled();
}

void MainWindow::on_pushButton_Cut_clicked()
{
    ui->textEdit->cut();
    ui->pushButton_Cut->setEnabled(false);
    ui->pushButton_Copy->setEnabled(false);
    ui->pushButton_Paste->setEnabled(true);
}

void MainWindow::on_pushButton_Paste_clicked()
{
    ui->textEdit->paste();
}

void MainWindow::on_pushButton_Undo_clicked()
{
    ui->textEdit->undo();
}

void MainWindow::on_pushButton_Redo_clicked()
{
    ui->textEdit->redo();
}

void MainWindow::on_pushButton_Exit_clicked()
{
    if(this->next!=NULL)
    {
        this->next->prev=NULL;
        this->next->on_pushButton_Exit_clicked();
    }
    if(this->prev!=NULL)
    {
        this->prev->next=NULL;
        this->prev->on_pushButton_Exit_clicked();
    }
    this->on_pushButton_Close_clicked();
}

void MainWindow::on_pushButton_Change_Font_clicked()
{
    bool Ok;
    this->Font = QFontDialog::getFont(&Ok, QFont("Times", 12), this, QString::fromUtf8("Choose font"));
    if (Ok)
    {
        ui->textEdit->setFont(this->Font);
        QSettings settings(QCoreApplication::applicationDirPath()+QDir::separator()+"settings.ini",QSettings::IniFormat);
        settings.setValue("Font", this->Font);
        settings.sync();
    }
}

void MainWindow::on_textEdit_textChanged()
{
    if(!this->isChanged)
    {
        this->Name=this->Name+"*";
        this->isChanged=true;
    }
    this->setWindowTitle(this->Name+" - MyTextEditor");
}

void MainWindow::on_actionAbout_Qt_triggered()
{
    QMessageBox::aboutQt(this, "About Qt");
}

void MainWindow::on_actionAbout_programm_triggered()
{
    QFile F(":/About.txt");
    if(!F.open(QIODevice::ReadOnly))
    {
        QMessageBox::critical(this, tr("Error"), tr("Can't open About"));
        return;
    }
    QTextStream in(&F);
    QMessageBox::about(this, "Info", in.readAll());
}

void MainWindow::on_textEdit_undoAvailable(bool b)
{
    ui->pushButton_Undo->setEnabled(b);
}

void MainWindow::on_textEdit_redoAvailable(bool b)
{
    ui->pushButton_Redo->setEnabled(b);
}

void MainWindow::on_textEdit_copyAvailable(bool b)
{
    ui->pushButton_Copy->setEnabled(b);
    ui->pushButton_Cut->setEnabled(b);
}

void MainWindow::on_actionSave_as_triggered()
{
    this->Path = QFileDialog::getSaveFileName(this, "Save as", "", "*.txt");
    this->Name=this->Path.split("/")[this->Path.split("/").length()-1];
    if (this->Name != "")
    {
        this->setWindowTitle(this->Name+" - MyTextEditor");
        QFile file(this->Name);
        if (!file.open(QIODevice::WriteOnly))
        {
            QMessageBox::critical(this,tr("Error"),tr("Couldn't open file "));
            return;
        }
        else
        {
            QTextStream stream(&file);
            stream << ui->textEdit->toPlainText();
            stream.flush();
            file.close();
            this->isSaved=true;
            this->isChanged=false;
        }
    }
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    qDebug()<<this->isChanged;
    qDebug()<<this->isSaved;
    if(!this->isChanged)
    {
        event->accept();
    }
    else if(!this->isSaved && !ui->textEdit->toPlainText().isEmpty())
    {
        QMessageBox::StandardButton que;
        que = QMessageBox::question(this, this->Name+QString::fromUtf8(" - MyTextEditor"),
                                    QString::fromUtf8("Do you want to save file before exit?"),
                                    QMessageBox::Yes |QMessageBox::No | QMessageBox::Cancel,QMessageBox::No);

        if(que==QMessageBox::Cancel)
        {
            event->ignore();
        }
        if(que==QMessageBox::Yes)
        {
            on_pushButton_Save_clicked();
            if(this->isSaved)
            {
                event->accept();
            }
            else
            {
                event->ignore();
            }
        }
        if (que==QMessageBox::No)
        {
            event->accept();
        }
    }
}
