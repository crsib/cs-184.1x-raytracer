#include "RTMainWindow.h"
#include "ui_RTMainWindow.h"

#include <QtCore>
#include <QtGui>
#include <QtWidgets>

#include <QtConcurrent>

RTMainWindow::RTMainWindow(QWidget *parent)
	: QMainWindow(parent), ui(new Ui::RTMainWindow)
{
	ui->setupUi( this );

	ui->gvResult->setScene(&m_GraphicsScene);
	m_CurrentPixmap = m_GraphicsScene.addPixmap( QPixmap() );

	m_LastUpdate = 0;

	connect( &m_ComputationResultWatcher, SIGNAL(progressRangeChanged(int,int)), ui->rtProgress, SLOT(setRange(int,int)) );
	connect( &m_ComputationResultWatcher, SIGNAL(progressValueChanged(int)), ui->rtProgress, SLOT(setValue(int)) );
	connect( &m_ComputationResultWatcher, SIGNAL(started()), SLOT(computationStarted()) );
	connect( &m_ComputationResultWatcher, SIGNAL(finished()), SLOT(computationFinished()) );
	connect( &m_ComputationResultWatcher, SIGNAL(canceled()), SLOT(computationCanceled()) );
	connect( &m_ComputationResultWatcher, SIGNAL(progressValueChanged(int)), SLOT(computationProgressValueChanged(int)) );

	QCoreApplication::setApplicationName( "raytracer" );
	QCoreApplication::setOrganizationName( "crsib" );

	QSettings settings;

	//ui->edtInputScene->setText( settings.value( "input_dir" ).toString() );
	ui->edtOutputDur->setText( settings.value( "output_dir" ).toString() );
}

RTMainWindow::~RTMainWindow()
{
	delete ui;
}

void RTMainWindow::on_btnBrowseScene_clicked()
{
	QSettings settings;
	QString path = QFileDialog::getOpenFileName( this, "Select scene file", settings.value( "input_dir" ).toString() );

	if( !path.isEmpty() )
	{
		ui->edtInputScene->setText( path );
		settings.setValue( "input_dir", QFileInfo( path ).absoluteDir().absolutePath() );
	}
}

void RTMainWindow::on_btnBrowseOutput_clicked()
{
	QSettings settings;
	QString path = QFileDialog::getExistingDirectory( this, "Select output", settings.value( "output_dir" ).toString() );

	if( !path.isEmpty() )
	{
		ui->edtOutputDur->setText( path );
		settings.setValue( "output_dir", QDir( path ).absolutePath() );
	}
}

void RTMainWindow::on_btnGenerate_clicked()
{
	if( m_ComputationResultWatcher.isRunning() )
	{
		m_ComputationResultWatcher.cancel();
		return ;
	}

	if( !m_Scene.loadSceneSynchronous( ui->edtInputScene->text(),
									  ui->edtWidth->text().toInt(),
									  ui->edtHeight->text().toInt(),
									  ui->cbOverrideSceneSize->isChecked() ) )
	{
		QMessageBox::critical( this, "Error", "Failed to load scene" );
		return ;
	}

	m_LastUpdate.store( 0 );

	const int width = m_Scene.sceneWidth();
	const int height= m_Scene.sceneHeight();

	m_CurrentImage = QImage( width, height, QImage::Format_RGB32 );
	updateInfo();

	QRgb* data = reinterpret_cast<QRgb*>( m_CurrentImage.bits() );

	rt::Scene* current_scene = &m_Scene;
	m_ComputationResultWatcher.setFuture( QtConcurrent::map( data, data + width * height,
					[width,height,data, current_scene]( QRgb& rgb )
					{
						const int offset = &rgb - data;
						const int i = offset % width;
						const int j = offset / width;
						rgb = current_scene->evaluatePixel( i, j );
					}
	) );
}

void RTMainWindow::computationStarted()
{
	ui->groupBox->setEnabled( false );
	ui->btnGenerate->setText( "Cancel" );
}

void RTMainWindow::computationFinished()
{
	ui->groupBox->setEnabled( true );
	ui->btnGenerate->setText( "Generate" );
	m_CurrentImage.save( QDir( ui->edtOutputDur->text() ).absoluteFilePath( m_Scene.outputFileName() ) );
	updateInfo();
}

void RTMainWindow::computationCanceled()
{
	ui->groupBox->setEnabled( true );
	ui->btnGenerate->setText( "Generate" );
	updateInfo();
}

void RTMainWindow::computationProgressValueChanged(int value )
{
	int max_diff = m_CurrentImage.width();
	int diff = value - m_LastUpdate.load();

	if( diff >= max_diff )
	{
		m_LastUpdate.store( value );
		updateInfo();
	}
}

void RTMainWindow::updateInfo()
{
	m_CurrentPixmap->setPixmap( QPixmap::fromImage( m_CurrentImage ) );
	statusBar()->showMessage( QString("Active threads: %1").arg( QThreadPool::globalInstance()->activeThreadCount() ) );
}
