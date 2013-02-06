#include "videobar.h"

VideoBar::VideoBar(QWidget *parent) : QWidget(parent)
{
  playButton = new QPushButton("Play");
  stopButton = new QPushButton("Stop");
  movieSeeker = new QSlider(Qt::Horizontal);
  hBox = new QHBoxLayout;
  playing = false;

  hBox->addWidget(playButton);
  hBox->addWidget(stopButton);
  hBox->addWidget(movieSeeker);

  connect(playButton,SIGNAL(clicked()),this,SLOT(hitPlay()));
  connect(stopButton,SIGNAL(clicked()),this,SIGNAL(stop()));
    
  this->setLayout(hBox);
}

VideoBar::~VideoBar()
{
  delete hBox;
}

void 
VideoBar::hitPlay()
{
  if ( playing ) emit pause();
  else emit play();
}

void
VideoBar::updateState(int newState)
{
  if ( newState == QMPwidget::PlayingState )
    {
      playButton->setText("Pause");
      playing = true;
    }
  else
    {
      playButton->setText("Play");
      playing = false;
      if ( newState == QMPwidget::NotStartedState )
	  movieSeeker->setSliderPosition(0);
    }
}
