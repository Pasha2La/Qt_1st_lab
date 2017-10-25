#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QClipboard>
#include <QString>
#include <QFileDialog>
#include <QFile>
#include <QMessageBox>
#include <QTextStream>
#include <QFont>
#include <QFontDialog>
#include <QSettings>
#include <QDebug>
#include <QDir>
#include <QCloseEvent>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    bool isSaved=false;
    bool isChanged = false;
    MainWindow *prev=NULL;
    MainWindow *next=NULL;
    QFont Font;
    QString Path;
    QString Name=QString("NewFile");
    bool ccAvailable=false;
    bool pAvailable=false;
    ~MainWindow();

private slots:
    void closeEvent(QCloseEvent *event);

    void on_pushButton_Create_clicked();

    void on_pushButton_Open_clicked();

    void on_pushButton_Save_clicked();

    void on_pushButton_Close_clicked();

    void on_pushButton_Copy_clicked();

    void on_pushButton_Paste_clicked();

    void on_pushButton_Cut_clicked();

    void on_pushButton_Undo_clicked();

    void on_pushButton_Redo_clicked();

    void on_pushButton_Exit_clicked();

    void on_pushButton_Change_Font_clicked();

    void on_textEdit_textChanged();

    void on_actionAbout_Qt_triggered();

    void on_actionAbout_programm_triggered();

    void on_textEdit_undoAvailable(bool b);

    void on_textEdit_redoAvailable(bool b);

    void on_textEdit_copyAvailable(bool b);

    void on_actionSave_as_triggered();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
