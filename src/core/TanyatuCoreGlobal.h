/*******************************************************************************
 * TanyatuCoreGlobal.h
 *
 * Copyright (c) 2012, Varuna L Amachi. All rights reserved.
 *
 * This program/library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This program/library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301  USA
 ******************************************************************************/

#ifndef TANYATUCOREGLOBAL_H
#define TANYATUCOREGLOBAL_H

#include <QtCore/QtGlobal>


#if defined(TANYATU_CORE)
 #define TANYATU_CORE_EXPORT Q_DECL_EXPORT
#else
 #define TANYATU_CORE_EXPORT Q_DECL_IMPORT
#endif


#endif // TANYATUCOREGLOBAL_H
