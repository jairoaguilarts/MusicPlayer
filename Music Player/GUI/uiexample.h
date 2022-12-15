#ifndef UIEXAMPLE_H
#define UIEXAMPLE_H

#include <QMainWindow>
#include <QDebug>
#include <QFileDialog>
#include <QStringList>
#include <QClipboard>
#include <QTime>
//#include <QMediaPlayer>
#include <QUrl>
#include "QMediaPlayer"
#include <QAudioOutput>
#include <iostream>
#include <vector>
#include "Genero.h"
#include "SongInfo.h"
#include "GeneroFileV.h"
#include "SongInfoFileV.h"
#include "PlayListFileV.h"
using namespace std;

namespace Ui {
class UIExample;
}

class UIExample : public QMainWindow
{
    Q_OBJECT

public:
    explicit UIExample(QWidget *parent = nullptr);
    ~UIExample();
    void crearVectores();
    void cargarCanciones();
    void cargarPlaylist();

private slots:

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_clicked();

private:

    Ui::UIExample *ui;
    QString ruta;
    vector<Genero*> generos;
    vector<SongInfo*> canciones;
    GeneroFileV *gfv = nullptr;
    SongInfoFileV *sifv = nullptr;
    PlayListFileV *plfv;

};

#endif // UIEXAMPLE_H
