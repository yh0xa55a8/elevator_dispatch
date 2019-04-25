//
// Created by 杨晗 on 2019-04-22.
//

#include "StatusButton.h"

StatusButton::StatusButton(const QString& text,QWidget *parent, QString tStyle, QString fStyle):
    QPushButton(text,parent),
    statusTrueStyle(std::move(tStyle)),
    statusFalseStyle(std::move(fStyle)){
    this->setStyleSheet(statusFalseStyle);
}