#ifndef RTMAINWINDOW_H
#define RTMAINWINDOW_H

#include <QMainWindow>
#include <QtCore/QFutureWatcher>

#include <QtGui/QImage>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>

#include <QAtomicInt>

#include "rt/Scene.h"

namespace Ui {
	class RTMainWindow;
}

class RTMainWindow : public QMainWindow
{
	Q_OBJECT

public:
	explicit RTMainWindow(QWidget *parent = 0);
	~RTMainWindow();

private slots:
	void on_btnBrowseScene_clicked();

	void on_btnBrowseOutput_clicked();

	void on_btnGenerate_clicked();

	void computationStarted();
	void computationFinished();
	void computationCanceled();
	void computationProgressValueChanged(int value);
private:
	void updateInfo();
	Ui::RTMainWindow *ui;

	QFutureWatcher<void>   m_ComputationResultWatcher;

	QGraphicsScene         m_GraphicsScene;
	QGraphicsPixmapItem*   m_CurrentPixmap;

	QImage                 m_CurrentImage;

	QAtomicInt             m_LastUpdate;

	rt::Scene              m_Scene;
};


#endif // RTMAINWINDOW_H
