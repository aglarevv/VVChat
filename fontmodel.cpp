#include "fontmodel.h"
#include "ui_fontmodel.h"

FontModel::FontModel(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FontModel)
{
    ui->setupUi(this);
}

FontModel::~FontModel()
{
    delete ui;
}

QWidget *FontModel::getWidget()
{
    return ui->widget;
}
