#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QSvgRenderer>
#include <QPainter>

#include <QPicture>

#include <QClipboard>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow), picture(nullptr) {
    ui->setupUi(this);

    ui->deHuDate->setDate(QDate::currentDate());

    connect(ui->leText, &QLineEdit::textChanged, this, &MainWindow::refreshPreview);
    connect(ui->cbNarrowFont, &QCheckBox::stateChanged, this, &MainWindow::refreshPreview);
    connect(ui->cbTextColor, &QComboBox::currentIndexChanged, this, &MainWindow::refreshPreview);
    connect(ui->cbType, &QComboBox::currentIndexChanged, this, &MainWindow::refreshPreview);
    connect(ui->deHuDate, &QDateEdit::dateChanged, this, &MainWindow::refreshPreview);
    connect(ui->cbStateEmblem, &QComboBox::currentIndexChanged, this, &MainWindow::refreshPreview);
    connect(ui->leDisctrictName, &QLineEdit::textChanged, this, &MainWindow::refreshPreview);
    connect(ui->sbImgHeight, &QSpinBox::valueChanged, this, &MainWindow::refreshPreview);

    refreshPreview();
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::refreshPreview() {
    bool narrowFont = ui->cbNarrowFont->isChecked();
    QString text = ui->leText->text();
    text = text.replace(" ", "  ");
    QString firstText = text.split(":")[0];

    bool hasStickers = text.contains(":");

    QFont f("Euro Plate", 800);
    QFontMetrics fm(f);
    int size = fm.size(Qt::TextSingleLine, text).width();
    if(narrowFont)
        size = size * 0.8;

    int stickerPosX  = fm.size(Qt::TextSingleLine, firstText).width();
    int stickerWidth = fm.size(Qt::TextSingleLine, ":").width();

    if(picture)
        delete picture;

    picture = new QPicture;
    QPainter *painter = new QPainter(picture);
    painter->setRenderHint(QPainter::Antialiasing, true);

    QRect border(15, 15, size + addSize, 970);
    QRect clip(29, 29, size + addSize, 942);
    painter->setClipRect(clip);

    renderEuEmblem(painter);

    painter->setClipping(false);

    painter->setPen(QPen(Qt::black, 30));
    painter->setBrush(QBrush());
    painter->drawRoundedRect(border, 50, 50);

    painter->setClipping(true);

    switch(ui->cbTextColor->currentIndex()) {
    case 1:  painter->setPen(Qt::red); break;
    case 2:  painter->setPen(QColor(0, 124, 12)); break;
    default: painter->setPen(Qt::black); break;
    }

    painter->setFont(f);
    if(narrowFont)
        painter->scale(0.8, 1);
    painter->drawText(550 + (narrowFont ? 150 : 0), 875, text);

    if(hasStickers)
        renderHuSticker(painter, QRect(677 + stickerPosX, 45, 360, 360));

    painter->end();
    delete painter;

    QPixmap pixmap(((addSize + size + 30) * 0.25), 250);
    pixmap.fill(Qt::white);
    QPainter p2(&pixmap);
    p2.scale(0.25, 0.25);
    picture->play(&p2);
    ui->lPreview->setPixmap(pixmap);

}

void MainWindow::renderEuEmblem(QPainter *p) {
    QFont font1("Euro Plate");
    font1.setPixelSize(1230);

    p->setBrush(QColor(255, 204, 0));
    p->drawRect(30, 0, 400, 600);

    p->setBrush(Qt::white);
    p->drawRect(30, 600, 400, 600);

    p->setPen(QColor(36, 63, 149));

    p->setFont(font1);
    p->drawText(-75, 950, "!");
}

void MainWindow::renderHuSticker(QPainter *p, QRect rect) {
    int year = ui->deHuDate->date().year();
    int month = ui->deHuDate->date().month();
    QImage img(":/HU-Sticker/" + QString::number(year) + ".png");

    int rotVal;
    if(month == 12)
        rotVal = 0;
    else
        rotVal = month * 30;

    if(rotVal % 90 != 0) {
        int value = 66;
        rect.translate(-value, -value);
        rect.setWidth(rect.width() + value*2);
        rect.setHeight(rect.height() + value*2);
    }

    img = img.scaled(QSize(rect.width(), rect.height()), Qt::KeepAspectRatio, Qt::SmoothTransformation);
    img = img.transformed(QTransform().rotate(rotVal), Qt::SmoothTransformation);
    p->drawImage(rect, img);
}

void MainWindow::on_pbCopyToClipboard_clicked() {
    QFont f("Euro Plate", 800);
    QFontMetrics fm(f);
    int size = fm.size(Qt::TextSingleLine, ui->leText->text()).width();
    QImage img(QSize(((addSize + size + 30)), 1000), QImage::Format_ARGB32);
    QPainter p(&img);

    img.fill(Qt::white);
    //p.scale(0.25, 0.25);

    picture->play(&p);

    QClipboard *c = QGuiApplication::clipboard();
    c->setImage(img);
}

