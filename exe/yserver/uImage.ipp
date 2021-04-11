////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

void UImage::init_scan()
{
    for(const char* z : Image::kSupportedExtensions){
        for(Fragment f : cdb::all_fragments_suffix(z))
            on_image_add(f);
        on_add("*." + QString(z), &UImage::on_image_add);
        on_modify("*." + QString(z), &UImage::on_image_modified);
        on_remove("*." + QString(z), &UImage::on_image_removed);
    }
}

void UImage::on_image_add(Fragment f)
{
    update(cdb::db_image(f));
}

void UImage::on_image_modified(Fragment f)
{
    update(cdb::db_image(f));
}

void UImage::on_image_removed(Fragment f)
{
    static thread_local SqlQuery d(wksp::cache(), "DELETE FROM Images WHERE id=?");
    auto d_af = d.af();
    d.bind(0, f.id);
    d.exec();
}

void UImage::update(Image i)
{
    if(!i.id)
        return ;
        
    //Document    doc(i.id);
    Fragment    frag{i.id}; //    = first(doc, DataRole::Image);
    QByteArray  byt     = cdb::bytes(frag);
    
    static thread_local SqlQuery u1(wksp::cache(), "UPDATE Images SET image=? WHERE id=?");
    auto u1_af  = u1.af();
    u1.bind(0, byt);
    u1.bind(1, i.id);
    u1.exec();
    
    ContentType ct  = cdb::type(i);
    if(ct == ContentType::svg)
        return ;

    QImageReader    reader(cdb::path(frag));
    QByteArray      fmt     = reader.format();
    
    auto ct2        = mimeTypeForExt(utf8(fmt));
    if(ct2 != ct){
            // Cover the case of a mis-labeled type
        static thread_local SqlQuery u2(wksp::cache(), "UPDATE Images SET type=? WHERE id=?");
        auto u2_af = u2.af();
        u2.bind(0, ct2.key());
        u2.bind(1, i.id);
        u2.exec();
    }
    
    QImage  img     = reader.read();
    QImage  sm      = img.scaled(Image::kSmallWidth,  Image::kSmallHeight,  Qt::KeepAspectRatio, Qt::SmoothTransformation);
    QImage  med     = img.scaled(Image::kMediumWidth, Image::kMediumHeight, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    QImage  large   = img.scaled(Image::kLargeWidth,  Image::kLargeHeight,  Qt::KeepAspectRatio, Qt::SmoothTransformation);
    
    QBuffer bs, bm, bl;
    bs.open(QIODevice::WriteOnly);
    sm.save(&bs, fmt.data());
    bs.close();
    bm.open(QIODevice::WriteOnly);
    med.save(&bm, fmt.data());
    bm.close();
    bl.open(QIODevice::WriteOnly);
    large.save(&bl, fmt.data());
    bl.close();
    
    static thread_local SqlQuery u3(wksp::cache(), "UPDATE Images SET small=?,medium=?,large=? WHERE id=?");
    auto u3_af = u3.af();
    u3.bind(0, bs.data());
    u3.bind(1, bm.data());
    u3.bind(2, bl.data());
    u3.bind(3, i.id);
    u3.exec();
}

