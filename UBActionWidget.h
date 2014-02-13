/*
 * Copyright (C) 2013-2014 Open Education Foundation
 *
 *
 * This file is part of OpenBoard.
 *
 * OpenBoard is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, version 3 of the License,
 * with a specific linking exception for the OpenSSL project's
 * "OpenSSL" library (or with modified versions of it that use the
 * same license as the "OpenSSL" library).
 *
 * OpenBoard is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with OpenBoard. If not, see <http://www.gnu.org/licenses/>.
 */


#ifndef UBACTIONWIDGET_H
#define UBACTIONWIDGET_H

#include <QWidget>

class UBActionWidget : public QWidget
{
    Q_OBJECT
public:
    explicit UBActionWidget(QWidget *parent = 0);

signals:
    void actionTrig();

private:
    virtual void showEvent(QShowEvent *e);

};

#endif // UBACTIONWIDGET_H
