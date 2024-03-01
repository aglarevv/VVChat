#ifndef FONTMODEL_H
#define FONTMODEL_H

#include <QWidget>

namespace Ui {
class FontModel;
}

class FontModel : public QWidget
{
    Q_OBJECT

public:
    explicit FontModel(QWidget *parent = nullptr);
    ~FontModel();


    QWidget * getWidget();
private:
    Ui::FontModel *ui;
};

#endif // FONTMODEL_H
