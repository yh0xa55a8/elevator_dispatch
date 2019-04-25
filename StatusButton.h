//
// Created by 杨晗 on 2019-04-22.
//

#ifndef OS_ASSIGNMENT1_STATUS_BUTTON_H
#define OS_ASSIGNMENT1_STATUS_BUTTON_H

#include <QPushButton>

// customize button that holds two style and can change between
class StatusButton: public QPushButton {
    Q_OBJECT
private:
    bool status = false;
    QString statusTrueStyle;
    QString statusFalseStyle;
public:
    explicit StatusButton(const QString& = "",
            QWidget * =nullptr,
            QString ="color:red;border:1px solid red;border-radius:3px;padding:4px 4px;",
            QString ="color:white;border:1px solid white;border-radius:3px;padding:4px 4px;");
    bool getStatus(){return status;};
public slots:
    void setStatus(bool b){
        if(status!=b){
          this->setStyleSheet(b?statusTrueStyle:statusFalseStyle);
           status=b;
        }
    };
};


#endif //OS_ASSIGNMENT1_STATUS_BUTTON_H
