#include "tabbutton.h"

#include <iostream>

TabButton::TabButton(QPushButton *parent) : QPushButton(parent)
{
  setFixedHeight(25);
  setMaximumWidth(120);
  setMinimumWidth(45);
  closeButton = new QPushButton("x",this);
  closeButton->setFixedSize(15,15);

  connect(closeButton,SIGNAL(clicked()),this,SIGNAL(removeTab()));
}

TabButton::~TabButton()
{
  delete closeButton;
}

void TabButton::resizeEvent(QResizeEvent *event) {
  closeButton->move(width()-20,5);
}
