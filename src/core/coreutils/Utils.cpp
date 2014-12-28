/*******************************************************************************
 * Utils.cpp
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

#include <QTime>
#include "Utils.h"

using namespace Tanyatu;

Data::MediaRating Utils::getRatingFromInt( int rating )
{
    switch(rating) {
    case 0: return Data::Rating_None;
    case 1: return Data::Rating_Bad;
    case 2: return Data::Rating_Average;
    case 3: return Data::Rating_Good;
    case 4: return Data::Rating_VeryGood;
    case 5: return Data::Rating_Excellent;
    }
    return Data::Rating_None;
}


QString Utils::getStringTime(int timeInSeconds) {
        int hours = timeInSeconds / (60 * 60);
        int min   = timeInSeconds / 60;
        int sec   = timeInSeconds % 60;
        QTime displayTime(hours , min, sec);
        return (hours == 0? displayTime.toString("mm:ss") :
                            displayTime.toString("hh:mm:ss"));
}


//QImage* Utils::imageForTrack( Data::AudioTrack *track )
//{
//    QImage *image = 0;
//    if( track->url().isLocalFile() )
//    {
//        TagLib::MPEG::File tagFile( track->url().toLocalFile()
//                                    .toStdString().c_str() );
//        if( tagFile.isValid() && tagFile.ID3v2Tag() ) {
//            TagLib::ID3v2::Tag *tag = tagFile.ID3v2Tag();
//            TagLib::ID3v2::FrameList l = tag->frameList("APIC");
//            if( ! l.isEmpty() ) {
//                image = new QImage();
//                TagLib::ID3v2::AttachedPictureFrame *frame =
//                        static_cast< TagLib::ID3v2::AttachedPictureFrame * >(
//                            l.front());
//                image->loadFromData( (const uchar *)
//                            frame->picture().data(), frame->picture().size());
//            }
//        }
//    }
//    return image;
//}

