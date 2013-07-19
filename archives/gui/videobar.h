#include <QPushButton>
#include <QHBoxLayout>
#include <QSlider>

#include "qmpwidget/qmpwidget.h"

class VideoBar : public QWidget {
  Q_OBJECT

public:
  VideoBar(QWidget *parent = 0);
  ~VideoBar();
  QSlider* seekSlider() const { return movieSeeker; }

signals:
  void play();
  void pause();
  void stop();

public slots:
  void hitPlay();
  void updateState(int);

private:
  bool playing;
  QPushButton *playButton, *stopButton;
  QSlider *movieSeeker;
  QHBoxLayout *hBox;

};
