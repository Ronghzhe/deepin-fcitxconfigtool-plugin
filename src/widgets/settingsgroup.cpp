/*
 * Copyright (C) 2016 ~ 2018 Deepin Technology Co., Ltd.
 *
 * Author:     sbw <sbw@sbw.so>
 *             kirigaya <kirigaya@mkacg.com>
 *             Hualet <mr.asianwang@gmail.com>
 *
 * Maintainer: sbw <sbw@sbw.so>
 *             kirigaya <kirigaya@mkacg.com>
 *             Hualet <mr.asianwang@gmail.com>
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

#include "settingsgroup.h"
#include "settingsitem.h"
#include "settingsheaderitem.h"
#include "settingshead.h"
#include "utils.h"
#include "imsettingsitem.h"
#include <DBackgroundGroup>

#include <QVBoxLayout>
#include <QEvent>
#include <QDebug>

DWIDGET_USE_NAMESPACE

namespace dcc_fcitx_configtool {
namespace widgets {

Fcitx_SettingsGroup::Fcitx_SettingsGroup(QFrame *parent, BackgroundStyle bgStyle)
    : Fcitx_TranslucentFrame(parent)
    , m_layout(new QVBoxLayout)
    , m_headerItem(nullptr)
{
    m_layout->setSpacing(5);
    m_layout->setContentsMargins(0, 0, 0, 0);

    QVBoxLayout *vLayout = m_layout;
    if (GroupBackground == bgStyle) {
        vLayout = new QVBoxLayout;
        m_bggroup = new DBackgroundGroup(m_layout);
        m_bggroup->setBackgroundRole(QPalette::Window);
        m_bggroup->setItemSpacing(1);
        m_bggroup->setUseWidgetBackground(false);
        vLayout->addWidget(m_bggroup);
        vLayout->setContentsMargins(0, 0, 0, 0);
    }

    m_bgStyle = bgStyle;
    setLayout(vLayout);
}

Fcitx_SettingsGroup::Fcitx_SettingsGroup(const QString &title, QFrame *parent)
    : Fcitx_SettingsGroup(parent)
{
    setHeaderVisible(!title.isEmpty());
    setAccessibleName(title);

    m_headerItem->setTitle(title);
}

Fcitx_SettingsGroup::~Fcitx_SettingsGroup()
{
    if (m_headerItem)
        m_headerItem->deleteLater();
}

void Fcitx_SettingsGroup::setHeaderVisible(const bool visible)
{
    if (visible) {
        if (!m_headerItem)
            m_headerItem = new Fcitx_SettingsHeaderItem;
        insertItem(0, m_headerItem);
    } else {
        if (m_headerItem) {
            m_headerItem->deleteLater();
            m_headerItem = nullptr;
        }
    }
}

void Fcitx_SettingsGroup::insertItem(const int index, Fcitx_SettingsItem *item)
{
    if (ItemBackground == m_bgStyle) {
        //当SettingsItem 被加入　Fcitx_SettingsGroup　时，为其加入背景
        item->addBackground();
    }

    m_layout->insertWidget(index, item);
    item->installEventFilter(this);

    Fcitx_IMSettingsItem *mItem = dynamic_cast<Fcitx_IMSettingsItem *>(item);
    if (mItem)
        connect(mItem, &Fcitx_IMSettingsItem::itemClicked, [=](Fcitx_IMSettingsItem *myItem) {
            int i = itemCount();
            for (int j = 0; j < i; ++j) {
                if (this->getItem(j) != myItem) {
                    Fcitx_IMSettingsItem *Titem = dynamic_cast<Fcitx_IMSettingsItem *>(this->getItem(j));
                    if (Titem) {
                        Titem->setItemSelected(false);
                    }
                }
            }
        });
}

void Fcitx_SettingsGroup::appendItem(Fcitx_SettingsItem *item)
{
    insertItem(m_layout->count(), item);
}

void Fcitx_SettingsGroup::appendItem(Fcitx_SettingsItem *item, BackgroundStyle bgStyle)
{
    if ((ItemBackground == bgStyle) && (m_bgStyle == ItemBackground)) {
        //当SettingsItem 被加入　Fcitx_SettingsGroup　时，为其加入背景
        item->addBackground();
    }

    m_layout->insertWidget(m_layout->count(), item);
    item->installEventFilter(this);

    Fcitx_IMSettingsItem *mItem = dynamic_cast<Fcitx_IMSettingsItem *>(item);
    if (mItem)
        connect(mItem, &Fcitx_IMSettingsItem::itemClicked, [=](Fcitx_IMSettingsItem *myItem) {
            int i = itemCount();
            for (int j = 0; j < i; ++j) {
                if (this->getItem(j) != myItem) {
                    Fcitx_IMSettingsItem *Titem = dynamic_cast<Fcitx_IMSettingsItem *>(this->getItem(j));
                    if (Titem) {
                        Titem->setItemSelected(false);
                    }
                }
            }
        });
}

void Fcitx_SettingsGroup::removeItem(Fcitx_SettingsItem *item)
{
    if (!item)
        return;
    m_layout->removeWidget(item);
    item->removeEventFilter(this);
}

void Fcitx_SettingsGroup::moveItem(Fcitx_SettingsItem *item, const int index)
{
    const int oldIndex = m_layout->indexOf(item);
    if (oldIndex == index)
        return;

    m_layout->removeWidget(item);
    m_layout->insertWidget(index, item);
}

void Fcitx_SettingsGroup::setSpacing(const int spaceing)
{
    m_layout->setSpacing(spaceing);
    if (m_bggroup)
        m_bggroup->setItemSpacing(spaceing);
}

int Fcitx_SettingsGroup::itemCount() const
{
    return m_layout->count();
}

void Fcitx_SettingsGroup::clear()
{
    const int index = m_headerItem ? 1 : 0;
    const int count = m_layout->count();

    for (int i(index); i != count; ++i) {
        QLayoutItem *item = m_layout->takeAt(index);
        QWidget *w = item->widget();
        w->removeEventFilter(this);
        w->setParent(nullptr);
        delete item;
        w->deleteLater();
    }
}

Fcitx_SettingsItem *Fcitx_SettingsGroup::getItem(int index)
{
    if (index < 0)
        return nullptr;

    if (index < itemCount()) {
        return qobject_cast<Fcitx_SettingsItem *>(m_layout->itemAt(index)->widget());
    }

    return nullptr;
}

void Fcitx_SettingsGroup::insertWidget(QWidget *widget)
{
    m_layout->insertWidget(m_layout->count(), widget);
}
} // namespace widgets
} // namespace dcc_fcitx_configtool
