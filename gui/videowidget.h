#include <QVBoxLayout>
#include <QFileDialog>

#include "qmpwidget/qmpwidget.h"
#include "videobar.h"

class VideoWidget : public QWidget {
  Q_OBJECT
  
public:
  VideoWidget(QWidget *parent = 0);
  ~VideoWidget();

signals:
  void qmpState(int);

public:
  void openFile(const QString &fileName);

private:
  QVBoxLayout *vBox;
  QMPwidget *videoViewer;
  VideoBar *actionBar;

};
