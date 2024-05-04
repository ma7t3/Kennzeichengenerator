#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <QPicture>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected slots:
    void refreshPreview();
    void renderEuEmblem(QPainter *);
    void renderHuSticker(QPainter *, QRect);

private slots:
    void on_pbCopyToClipboard_clicked();

private:
    Ui::MainWindow *ui;

    const int addSize = 650;

    QPicture *picture;
};
#endif // MAINWINDOW_H
