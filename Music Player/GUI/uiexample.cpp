#include "uiexample.h"
#include "ui_uiexample.h"

UIExample::UIExample(QWidget *parent):QMainWindow(parent), ui(new Ui::UIExample)
{
    ui->setupUi(this);

    //Cambiar rutas
    //rutas mac jairo
    //this->gfv = new GeneroFileV("/Users/jairoaguilar/Documents/Clases/2022\ Q4/Estructura\ de\ Datos\ II/Proyecto/Music\ Player/Music\ Player/GUI/Generos.txt");
    //this->sifv = new SongInfoFileV("/Users/jairoaguilar/Documents/Clases/2022\ Q4/Estructura\ de\ Datos\ II/Proyecto/Music\ Player/Music\ Player/GUI/Canciones.txt");
    //this->plfv = new PlayListFileV("/Users/jairoaguilar/Documents/Clases/2022\ Q4/Estructura\ de\ Datos\ II/Proyecto/Music\ Player/Music\ Player/GUI/Playlists.txt");

    //rutas linux shell0
    //this->gfv = new GeneroFileV("/home/shell0/Documents/MusicPlayer/Music Player/GUI/Generos.txt");
    //this->sifv = new SongInfoFileV("/home/shell0/Documents/MusicPlayer/Music Player/GUI/Canciones.txt");

    //rutas mac shell0
    this->gfv = new GeneroFileV("/Users/marcellomenjivarmontesdeoca/Documents/Unitec/MusicPlayer/Music Player/Archivos para el proyecto/Generos.txt");
    this->sifv = new SongInfoFileV("/Users/marcellomenjivarmontesdeoca/Documents/Unitec/MusicPlayer/Music Player/Archivos para el proyecto/Canciones.txt");
    this->plfv = new PlayListFileV("/Users/marcellomenjivarmontesdeoca/Documents/Unitec/MusicPlayer/Music Player/Archivos para el proyecto/Playlists.txt");

    //rutas windows Julio
    //this->gfv = new GeneroFileV("C:/Users/jyahi/OneDrive/Escritorio/Codigo\ Estructura\ de\ Datos/Proyecto\ GUI/MusicPlayer/Music\ Player/GUI/Generos.txt");
    //this->sifv = new SongInfoFileV("C:/Users/jyahi/OneDrive/Escritorio/Codigo\ Estructura\ de\ Datos/Proyecto\ GUI/MusicPlayer/Music\ Player/GUI/Canciones.txt");
    //this->plfv = new PlayListFileV("C:/Users/jyahi/OneDrive/Escritorio/Codigo\ Estructura\ de\ Datos/Proyecto\ GUI/MusicPlayer/Music\ Player/GUI/Playlists.txt");

    //Rutas Windows Fran
    //this->gfv = new GeneroFileV("C:/Users/JOSE\ VILLEDA/Documents/UNITEC/2022\ Q4/ESTRUCTRA\ DE\ DATOS\ II/New\ folder\ (6)/MusicPlayer/Music\ Player/GUI/Generos.txt");
    //this->sifv = new SongInfoFileV("C:/Users/JOSE\ VILLEDA/Documents/UNITEC/2022\ Q4/ESTRUCTRA\ DE\ DATOS\ II/New\ folder\ (6)/MusicPlayer/Music\ Player/GUI/Canciones.txt");
    //this->plfv = new PlayListFileV("C:/Users/JOSE\ VILLEDA/Documents/UNITEC/2022\ Q4/ESTRUCTRA\ DE\ DATOS\ II/New\ folder\ (6)/MusicPlayer/Music\ Player/GUI/Playlists.txt");

    crearVectores();
    cargarCanciones();
    cargarPlaylist();
    this->ShuffleFlag = false;
    this->RepeatStat = RepeatFlags::All;

    //connect(ui->actionPlay_Selected, SIGNAL(triggered()), ui->btnPlay, SLOT(click()));
    mPlayer = new QMediaPlayer();
    aOutput = new QAudioOutput();
    connect(mPlayer, SIGNAL(positionChanged(qint64)), this, SLOT(ElapsedTime(qint64)));
    connect(mPlayer, SIGNAL(durationChanged(qint64)), this, SLOT(RemaningTime(qint64)));

    mPlayer->setAudioOutput(aOutput);

}

UIExample::~UIExample()
{

    gfv->cerrar();
    sifv->cerrar();
    delete ui;
}

void UIExample::on_btnSeleccionarRuta_clicked()
{
    //QStringList list  = QFileDialog::getOpenFileNames(this, tr("Select Files"), "Z:\\Music", tr("MP3 Files (*.mp3)"));
    QStringList list  = QFileDialog::getOpenFileNames(this, tr("Select Files"), "~/", tr("MP3 Files (*.mp3)"));
    if(list.isEmpty())
        return;
    foreach(QString rutaSeleccionada, list)
    {
        this->ruta = rutaSeleccionada;
    }
}

void UIExample::on_btnCrearCancion_clicked()
{
    //Obtiene los datos de los lineEdits
    QString nombre = ui->lineNombreCancion->text();
    QString disco = ui->lineDisco->text();
    QString artistas = ui->lineArtistas->text();
    QString genero = ui->lineGenero->text();

    if(ruta.toStdString().size() != 0 && nombre.toStdString().size() != 0 && disco.toStdString().size() != 0 && artistas.toStdString().size() != 0) {

        this->sifv->abrir();
        SongInfo *cancion = new SongInfo(nombre.toStdString(), disco.toStdString(), artistas.toStdString(), ruta.toStdString(), genero.toStdString());

        //clear el UI y la ruta
        ui->lineNombreCancion->clear();
        ui->lineDisco->clear();
        ui->lineArtistas->clear();
        this->ruta = "";

        this->canciones.push_back(cancion);
        this->sifv->agregarCancion(cancion);

        //Inserta a la tabla canciones
        ui->tablaCanciones->insertRow(ui->tablaCanciones->rowCount());
        ui->tablaCanciones->setItem(ui->tablaCanciones->rowCount() - 1, 0, new QTableWidgetItem(nombre));

        //aqui escribe la cancion
        sifv->escribir();
        sifv->cerrar();

    } else {

        // Mostrar dialogo para indicar que no se ha seleccionado ninguna ruta
        QMessageBox msgBox;
        msgBox.setText("Complete todos los campos de la cancion por favor.");
        msgBox.exec();

    }
}

void UIExample::on_btnCrearGenero_clicked()
{
    QString Qgenero = ui->lineGenero->text();
    if(Qgenero.toStdString().size() != 0) {
        gfv->abrir();
        Genero *genero = new Genero(Qgenero.toStdString());
        ui->lineGenero->clear();
        generos.push_back(genero);
        gfv->agregarGenero(genero);
        //agrega el genero al archivo genero.txt
        gfv->escribir();
    } else {
        // Mostrar dialogo para indicar que no se ha seleccionado ninguna ruta
        QMessageBox msgBox;
        msgBox.setText("Inserte el nombre del genero por favor.");
        msgBox.exec();
    }
}

void UIExample::crearVectores() {

    //carga los generos de Generos.txt
    if(gfv->abrir()) {
        this->gfv->leer();
        vector<Object*> generosLeidos = gfv->getGeneros();
        for(int i = 0; i < generosLeidos.size(); i++) { //Castea de Object* a Genero*
            Genero *gen = dynamic_cast<Genero*>(generosLeidos[i]);
            this->generos.push_back(gen);
        }
        gfv->cerrar();
    }

    //carga las canciones de Canciones.txt
    if(sifv->abrir()) {
        this->sifv->leer();
        vector<Object*> cancionesLeidas = sifv->getCanciones();
        for(int i = 0; i < cancionesLeidas.size(); i++) { //Castea de Object* a SongInfo*
            SongInfo *can = dynamic_cast<SongInfo*>(cancionesLeidas[i]);
            this->canciones.push_back(can);
        }
        sifv->cerrar();
    }

    //carga los playlists de Playlist.txt
    if(plfv->abrir()) {
        this->plfv->leer();
        vector<Object*> playlistsLeidas = plfv->getPlaylists();
        for(int i = 0; i < playlistsLeidas.size(); i++) {
            PlayListInfo* lista = dynamic_cast<PlayListInfo*>(playlistsLeidas[i]);
            this->pListas.push_back(lista);
        }
        plfv->cerrar();
    }
}

void UIExample::cargarCanciones() {
    for(int i = 0; i < canciones.size(); i++) {
        QString nombre(canciones[i]->getNombre().c_str());
        ui->tablaCanciones->insertRow(ui->tablaCanciones->rowCount());
        ui->tablaCanciones->setItem(ui->tablaCanciones->rowCount() - 1, 0, new QTableWidgetItem(nombre));
    }
    ui->tablaCanciones->resizeColumnToContents(0);
}

void UIExample::cargarPlaylist(){
    for(int i = 0; i < pListas.size(); i++){
        QString nombre(pListas[i]->getNombre().c_str());
        ui->tablaPlaylist->insertRow(ui->tablaPlaylist->rowCount());
        ui->tablaPlaylist->setItem(ui->tablaPlaylist->rowCount() - 1, 0, new QTableWidgetItem(nombre));
    }
    ui->tablaPlaylist->resizeColumnToContents(0);
}

void UIExample::on_btnPlay_clicked()
{
    QItemSelectionModel *selected = ui->tablaCanciones->selectionModel();
    if(ui->tablaCanciones->item(0, 0) ==0)
        return;

    int m_current_row=0;

    if(ShuffleFlag)
    {
        int randomValue = rand() % ui->tablaCanciones->rowCount();
        ui->tablaCanciones->selectRow(randomValue);
    }

    if(selected->hasSelection())
    {
        m_current_row = selected->currentIndex().row();

    }else{
        ui->tablaCanciones->selectRow(m_current_row);
    }

    ui->statusbar->showMessage("[+]Now Playing: " + ui->tablaCanciones->item(m_current_row, 0)->text(), 20*1000);
    if(!isPlayingPlaylist) {
        SongInfo *cancion = canciones[m_current_row];
        mPlayer->setSource(QUrl::fromLocalFile(QString(cancion->getRuta().c_str())));
        mPlayer->play();
    } else {
        SongInfo *cancion = cancionesPlaylist[m_current_row];
        mPlayer->setSource(QUrl::fromLocalFile(QString(cancion->getRuta().c_str())));
        mPlayer->play();
    }

}

void UIExample::ElapsedTime(qint64 x)
{

    QTime t = QTime(0,0,0);
    t =  t.addMSecs(x);
    ui->lblTimeElsp->setText(t.toString("mm:ss"));
    ui->SeekSlider->setValue(x);
    if(x!=0 && x==mPlayer->duration() )
    {
        if(RepeatStat == RepeatFlags::None)
        {
            mPlayer->stop();
            return;
        }

        on_btnNext_clicked();
    }

}
/// Slot to update interface
void UIExample::RemaningTime(qint64 x)
{
    QTime t = QTime(0,0,0);
    t =  t.addMSecs(x);
    ui->lblTimeRemaining->setText(t.toString("mm:ss"));
    ui->SeekSlider->setRange(0, x);
}

void UIExample::on_btnNext_clicked()
{
    QItemSelectionModel *selected = ui->tablaCanciones->selectionModel();
    int m_current_row = selected->currentIndex().row();

    if(ui->tablaCanciones->item(0, 0) ==0  &&  m_current_row == 0)
        return;

    if(RepeatStat == RepeatFlags::One)
    {
        ui->tablaCanciones->selectRow(m_current_row);

    }else if((RepeatStat == RepeatFlags::All) && (m_current_row == ui->tablaCanciones->rowCount()-1))
    {
        ui->tablaCanciones->selectRow(0);
    }else{
        ui->tablaCanciones->selectRow(m_current_row+1);
    }
    on_btnPlay_clicked();
}

void UIExample::on_btnStop_clicked()
{
    mPlayer->stop();
}

void UIExample::on_btnShuffel_clicked()
{
    if(ui->btnShuffel->isChecked())
    {
        ShuffleFlag = true;
        ui->statusbar->showMessage("Shuffle selection is On", 5*1000);
    }else{
        ShuffleFlag = false;
        ui->statusbar->showMessage("Shuffle selection is Off", 5*1000);
    }
}

void UIExample::on_btnPrev_clicked()
{
    QItemSelectionModel *selected = ui->tablaCanciones->selectionModel();
    int m_current_row = selected->currentIndex().row();

    if(ui->tablaCanciones->item(0, 0) ==0  &&  m_current_row == 0)
        return;

    if(RepeatStat == RepeatFlags::One)
    {
        ui->tablaCanciones->selectRow(m_current_row);

    }else if( (RepeatStat == RepeatFlags::All) && (m_current_row == 0) )
    {
        ui->tablaCanciones->selectRow(ui->tablaCanciones->rowCount()-1);
    }else{
        ui->tablaCanciones->selectRow(m_current_row-1);
    }
    on_btnPlay_clicked();
}

void UIExample::on_btnPause_clicked()
{
    if(ui->btnPause->isChecked())
    {
        mPlayer->pause();
    }else{
        mPlayer->play();
    }
}

void UIExample::on_btnRepeat_clicked()
{
    RepeatStat++;

    if(RepeatStat == RepeatFlags::One )
    {
        ui->btnRepeat->setChecked(true);
        ui->statusbar->showMessage("Repeat One", 5*1000);
    }else if(RepeatStat == RepeatFlags::All)
    {
        ui->btnRepeat->setChecked(true);
        ui->statusbar->showMessage("Repeat All", 5*1000);
    }else{
        ui->btnRepeat->setChecked(false);
        ui->statusbar->showMessage("Repeat Off", 5*1000);
        RepeatStat=0;
    }
}

void UIExample::on_btnMute_clicked()
{
    if(ui->btnMute->isChecked())
    {
        aOutput->setMuted(true);
    } else {
        aOutput->setMuted(false);
    }
}

//  !!!!!!!!!AQUIIIII!!!!!!!!!!!!!!!!
void UIExample::on_btnCrearPlaylist_clicked()
{
    //Agarra el nombre de la playlist
    QString playlistName = ui->lineNombrePlaylist->text();

    //valida
    if(playlistName.toStdString().size() != 0) {

        this->plfv->abrir();

        //clear el UI y la ruta
        ui->lineNombrePlaylist->clear();
        PlayListInfo *playList = new PlayListInfo(playlistName.toStdString());

        //Agrega a la tabla de playlists
        ui->tablaPlaylist->insertRow(ui->tablaPlaylist->rowCount());
        ui->tablaPlaylist->setItem(ui->tablaPlaylist->rowCount() - 1, 0, new QTableWidgetItem(playlistName));

        this->pListas.push_back(playList);
        this->plfv->agregarPlayList(playList);

        //aqui escribe la playlist
        plfv->escribir();
        plfv->cerrar();

    } else {

        // Mostrar dialogo para indicar que no se ha seleccionado ninguna ruta
        QMessageBox msgBox;
        msgBox.setText("Complete todos los campos de la cancion por favor.");
        msgBox.exec();

    }
}

void UIExample::on_SeekSlider_sliderMoved(int position)
{
    mPlayer->setPosition(position);
}


void UIExample::on_bttnShowPlay_clicked()
{
    QItemSelectionModel *playlistSelected = ui->tablaPlaylist->selectionModel();

    if(ui->tablaPlaylist->item(0, 0) == 0)
        return;

    int m_current_row=0;
    if(playlistSelected->hasSelection())
    {
        m_current_row = playlistSelected->currentIndex().row();

    }else{
        ui->tablaPlaylist->selectRow(m_current_row);
    }

    vector<Object*> tempCanciones = pListas[m_current_row]->getCanciones();
    for(int i = 0; i < tempCanciones.size(); i++) {
        SongInfo *cancion = dynamic_cast<SongInfo*>(tempCanciones[i]);
        this->cancionesPlaylist.push_back(cancion);
    }
    tempCanciones.clear();
    this->isPlayingPlaylist = true;

    PlayListInfo *listaSelected = pListas[m_current_row];

    ui->tablaCanciones->setRowCount(0);
    for(int i = 0; i < listaSelected->getCanciones().size(); i++) {
        SongInfo *cancion = dynamic_cast<SongInfo*>(listaSelected->getCanciones()[i]);
        QString nombre(cancion->getNombre().c_str());
        ui->tablaCanciones->insertRow(ui->tablaCanciones->rowCount());
        ui->tablaCanciones->setItem(ui->tablaCanciones->rowCount() - 1, 0, new QTableWidgetItem(nombre));
    }
    ui->tablaCanciones->resizeColumnToContents(0);
}


void UIExample::on_bttnShowSongs_clicked()
{
    this->isPlayingPlaylist = false;
    ui->tablaCanciones->setRowCount(0);
    cargarCanciones();
}


void UIExample::on_btnAgregarCancio_clicked()
{
    QItemSelectionModel *playlistSelected = ui->tablaPlaylist->selectionModel();
    QItemSelectionModel *cancionSelected = ui->tablaCanciones->selectionModel();

    if(ui->tablaPlaylist->item(0, 0) == 0 || ui->tablaCanciones->item(0, 0) == 0)
        return;

    int m_current_row_song = 0, m_current_row_playlist = 0;

    if(playlistSelected->hasSelection() && cancionSelected->hasSelection())
    {
        m_current_row_playlist = playlistSelected->currentIndex().row();
        m_current_row_song = cancionSelected->currentIndex().row();
    }else{
        ui->tablaPlaylist->selectRow(m_current_row_playlist);
        ui->tablaCanciones->selectRow(m_current_row_song);
    }

    PlayListInfo *listaSelected = pListas[m_current_row_playlist];
    SongInfo *songSelected = canciones[m_current_row_song];
    listaSelected->addCancion(songSelected);

    QMessageBox msgBox;
    string mensaje(songSelected->getNombre() + " fue agregada a a la playlist: " + listaSelected->getNombre());
    msgBox.setText(mensaje.c_str());
    msgBox.exec();
    plfv->abrir();
    plfv->escribir();
    plfv->cerrar();
}

