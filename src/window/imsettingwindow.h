/*
* Copyright (C) 2021 ~ 2021 Deepin Technology Co., Ltd.
*
* Author:     liuwenhao <liuwenhao@uniontech.com>
*
* Maintainer: liuwenhao <liuwenhao@uniontech.com>
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#ifndef IMSETTINGWINDOW_H
#define IMSETTINGWINDOW_H

#include "fcitxInterface/global.h"
#include <QWidget>

namespace dcc {
namespace widgets {
class SettingsGroup;
class ComboxWidget;
} // namespace widgets
} // namespace dcc

namespace Dtk {
namespace Widget {
class DListView;
class DCommandLinkButton;
class DFloatingButton;
} // namespace Widget
} // namespace Dtk

class ComBoboxSettingsItem;
class KeySettingsItem;

class IMSettingWindow : public QWidget
{
    Q_OBJECT
public:
    explicit IMSettingWindow(QWidget *parent = nullptr);
    virtual ~IMSettingWindow();
    void updateUI(); //刷新界面
signals:
    void popIMAddWindow(); //弹出添加输入法界面
private:
    void initUI(); //初始化界面
    void initConnect(); //初始化信号槽
    void readConfig(); //读取配置文件
private slots:
    void onEditBtnClicked(); //启用编辑
    void onDefaultIMChanged();
    void onCurIMChanged(FcitxQtInputMethodItemList list);
    void onAddBtnCilcked(); //

private:
    dcc::widgets::ComboxWidget *m_defaultIMCbox {nullptr}; //选择默认输入法
    Dtk::Widget::DCommandLinkButton *m_editBtn {nullptr}; //编辑
    Dtk::Widget::DListView *m_IMCurrentView {nullptr}; //当前输入法列表
    dcc::widgets::SettingsGroup *m_shortcutGroup {nullptr}; //快捷键容器
    ComBoboxSettingsItem *m_imSwitchCbox {nullptr}; //切换输入法（快捷键）
    KeySettingsItem *m_virtualKey {nullptr}; //虚拟键盘 （快捷键）
    KeySettingsItem *m_defaultIMKey {nullptr}; //默认输入法 （快捷键）
    dcc::widgets::ComboxWidget *m_systemAppCbox {nullptr}; //切换系统/应用
    Dtk::Widget::DFloatingButton *m_addIMBtn {nullptr}; //添加输入法
};

#endif // IMSETTINGWINDOW_H
