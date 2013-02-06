#include "videowidget.h"

VideoWidget::VideoWidget(QWidget *parent) : QWidget(parent) 
{
  vBox = new QVBoxLayout;

  videoViewer = new QMPwidget;
  videoViewer->setMinimumSize(600,300);
  actionBar = new VideoBar;

  vBox->addWidget(videoViewer);
  vBox->addWidget(actionBar);

  connect(actionBar,SIGNAL(play()),videoViewer,SLOT(play()));
  connect(actionBar,SIGNAL(pause()),videoViewer,SLOT(pause()));
  connect(actionBar,SIGNAL(stop()),videoViewer,SLOT(stop()));

  connect(videoViewer,SIGNAL(stateChanged(int)),
	  actionBar,SLOT(updateState(int)));
  connect(videoViewer,SIGNAL(stateChanged(int)),this,SIGNAL(qmpState(int)));

  videoViewer->start();

  videoViewer->setSeekSlider(actionBar->seekSlider());

  this->setLayout(vBox);
}

VideoWidget::~VideoWidget()
{
  delete vBox;
}


void
VideoWidget::openFile(const QString &fileName)
{
  videoViewer->load(fileName);
}
