#include <QVBoxLayout>
#include <QFileDialog>
#include <QStackedWidget>
#include <QSignalMapper>
#include <QVector>
#include <QPair>
#include <QScrollArea>

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
  QWidget *all_tabs_widget;
  QHBoxLayout *all_tabs;
  QScrollArea *all_tabs_container;
  QSignalMapper *tabsToStack, *tabsToVec;
  QPushButton *addTab;
  QStackedWidget *videos;
  QVector<QPair<QMPwidget*,TabButton*> > videoViewers;
  VideoBar *actionBar;

};
