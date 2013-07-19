#include "videowidget.h"

#include <iostream>

VideoWidget::VideoWidget(QWidget *parent) : QWidget(parent) 
{
  vBox = new QVBoxLayout;
  videos = new QStackedWidget;
  actionBar = new VideoBar;
  tabs = new QHBoxLayout;

  // Starts ScrollArea container definition
  // ScrollArea Heirarchy setup
  all_tabs_widget = new QWidget;
  all_tabs = new QHBoxLayout(all_tabs_widget);
  all_tabs_container = new QScrollArea;
  all_tabs_container->setWidget(all_tabs_widget);
  
  // Manipulation of size etc.
  all_tabs->addStrut(550);
  tabs->addStrut(50);
  all_tabs_container->setFixedSize(550, 50);
  all_tabs_widget->setFixedSize(550, 50);
  // Ends ScrollArea container deinition
  
  addTab = new QPushButton("+");
  tabsToStack = new QSignalMapper(this);
  tabsToVec = new QSignalMapper(this);
  addTab->setFixedSize(25,25);

  tabs->addWidget(all_tabs_container);
  tabs->addWidget(addTab);
  tabs->setSpacing(0);
  connect(addTab,SIGNAL(clicked()),this,SLOT(newTab()));
  
  // Setting up the first tab
  newTab();
  
  // vertical layout
  vBox->addLayout(tabs);
  vBox->addWidget(videos);
  vBox->addWidget(actionBar);

  connect(tabsToStack,SIGNAL(mapped(int)),videos,SLOT(setCurrentIndex(int)));
  connect(tabsToVec,SIGNAL(mapped(int)),this,SLOT(destroy(int)));
  
  this->setLayout(vBox);
}

VideoWidget::~VideoWidget()
{
  for ( int i = 0; i < videoViewers.size(); ++i ) 
    {
      delete videoViewers[i].first; 
      delete videoViewers[i].second;
    }
  delete vBox;
}


void
VideoWidget::openFile(const QString &fileName)
{
  videoViewers[videos->currentIndex()].first->start();
  videoViewers[videos->currentIndex()].first->load(fileName);
}

void
VideoWidget::newTab()
{
  // Allocate memory for the video tab and the button.
  QMPwidget *videoViewer = new QMPwidget;
  videoViewer->setMinimumSize(600,300);
  videos->addWidget(videoViewer);
  TabButton *newTab = new TabButton();

  //all_tabs_container->setFixedSize((all_tabs->count()+1)*80, 40);
  newTab->setFixedSize(120,25);
  all_tabs_widget->setFixedSize((all_tabs->count()+1)*90, 40);
  all_tabs->insertWidget(all_tabs->count()-1,newTab);
  
  // connect the tabs up so that it switches properly.
  connect(newTab,SIGNAL(clicked()),tabsToStack,SLOT(map()));
  connect(newTab,SIGNAL(removeTab()),tabsToVec,SLOT(map()));

  tabsToStack->setMapping(newTab,videos->indexOf(videoViewer));
  tabsToVec->setMapping(newTab,videoViewers.size());

  connect(videoViewer,SIGNAL(stateChanged(int)),
	  actionBar,SLOT(updateState(int)));
  connect(videoViewer,SIGNAL(stateChanged(int)),this,SIGNAL(qmpState(int)));

  connect(actionBar,SIGNAL(play()),videoViewer,SLOT(play()));
  connect(actionBar,SIGNAL(pause()),videoViewer,SLOT(pause()));
  connect(actionBar,SIGNAL(stop()),videoViewer,SLOT(stop()));

  videoViewer->start();
  videoViewer->setSeekSlider(actionBar->seekSlider());

  videoViewers.push_back(QPair<QMPwidget*,TabButton*>(videoViewer,newTab));

  videos->setCurrentWidget(videoViewer);
}

void VideoWidget::destroy(int destroyTab)
{
  /* trying to delete last frame */
  if ( videoViewers.size() <= 1 ) exit(1);

  QMPwidget *video = videoViewers[destroyTab].first;
  TabButton *tab = videoViewers[destroyTab].second;
  videos->removeWidget(video);
  
  delete video;
  delete tab;
  
  videoViewers.erase(QVector<QPair<QMPwidget*,TabButton*> >::
		     iterator(videoViewers.begin()+destroyTab));
}
