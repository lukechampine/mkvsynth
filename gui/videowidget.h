#include <QVBoxLayout>
#include <QFileDialog>
#include <QStackedWidget>
#include <QSignalMapper>
#include <QVector>
#include <QPair>

#include "qmpwidget/qmpwidget.h"
#include "videobar.h"
#include "tabbutton.h"

class VideoWidget : public QWidget {
  Q_OBJECT
  
public:
  VideoWidget(QWidget *parent = 0);
  ~VideoWidget();

signals:
  void qmpState(int);

public slots:
  void openFile(const QString &);
  void newTab();
  void destroy(int);

private:
  QVBoxLayout *vBox;
  QHBoxLayout *tabs;
  QSignalMapper *tabsToStack, *tabsToVec;
  QPushButton *addTab;
  QStackedWidget *videos;
  QVector<QPair<QMPwidget*,TabButton*> > videoViewers;
  VideoBar *actionBar;

};
