#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDebug>
#include <QFileDialog>
#include <QStringList>
#include <QClipboard>
#include <QTime>
#include <QMediaPlayer>
#include <QUrl>


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_btnRepeat_clicked();
    void on_btnShuffel_clicked();
    void on_actionAdd_files_triggered();
    void on_btnPlay_clicked();
    void on_actionCopy_to_Clipboard_triggered();
    void on_actionRemove_File_triggered();
    void on_actionClear_all_triggered();
    void on_actionAdd_to_Queue_triggered();

    void on_btnNext_clicked();
    void on_btnPrev_clicked();
    void on_btnStop_clicked();
    void on_btnPause_clicked();
    void on_btnMute_clicked();

    void on_SeekSlider_sliderMoved(int position);
    void on_VolumeSlider_sliderMoved(int position);

    void ElapsedTime(qint64 x);
    void RemaningTime(qint64 x);

private:
    Ui::MainWindow *ui;

    enum RepeatFlags {None = 0, One = 1, All =2};

    bool ShuffleFlag;
    bool isPlayingFlag;
    int RepeatStat;

    QMediaPlayer *mPlayer;
};

#endif // MAINWINDOW_H
