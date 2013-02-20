#include <QPushButton>

class TabButton : public QPushButton {
  Q_OBJECT

public:
  TabButton(QPushButton *parent = 0 );
  ~TabButton();

protected:
  void resizeEvent(QResizeEvent *event);

private:
  QPushButton *closeButton;

signals:
  void removeTab();

};
