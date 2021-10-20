#include <iostream>
#include <OCIdl.h>
#include <OleCtl.h>
#include "image.h"
#include "../lodepng/lodepng.h"

// https://raw.githubusercontent.com/lvandeve/lodepng/master/examples/example_bmp2png.cpp
unsigned decodeBMP(std::vector<unsigned char>& image, unsigned& w, unsigned& h, const std::vector<unsigned char>& bmp) {
    static const unsigned MINHEADER = 54; //minimum BMP header size

    if (bmp.size() < MINHEADER) return -1;
    if (bmp[0] != 'B' || bmp[1] != 'M') return 1; //It's not a BMP file if it doesn't start with marker 'BM'
    unsigned pixeloffset = bmp[10] + 256 * bmp[11]; //where the pixel data starts
    //read width and height from BMP header
    w = bmp[18] + bmp[19] * 256;
    h = bmp[22] + bmp[23] * 256;
    //read number of channels from BMP header
    if (bmp[28] != 24 && bmp[28] != 32) return 2; //only 24-bit and 32-bit BMPs are supported.
    unsigned numChannels = bmp[28] / 8;

    //The amount of scanline bytes is width of image times channels, with extra bytes added if needed
    //to make it a multiple of 4 bytes.
    unsigned scanlineBytes = w * numChannels;
    if (scanlineBytes % 4 != 0) scanlineBytes = (scanlineBytes / 4) * 4 + 4;

    unsigned dataSize = scanlineBytes * h;
    if (bmp.size() < dataSize + pixeloffset) return 3; //BMP file too small to contain all pixels

    image.resize(w * h * 4);

    /*
    There are 3 differences between BMP and the raw image buffer for LodePNG:
    -it's upside down
    -it's in BGR instead of RGB format (or BRGA instead of RGBA)
    -each scanline has padding bytes to make it a multiple of 4 if needed
    The 2D for loop below does all these 3 conversions at once.
    */
    for (unsigned y = 0; y < h; y++)
        for (unsigned x = 0; x < w; x++) {
            //pixel start byte position in the BMP
            unsigned bmpos = pixeloffset + (h - y - 1) * scanlineBytes + numChannels * x;
            //pixel start byte position in the new raw image
            unsigned newpos = 4 * y * w + 4 * x;
            if (numChannels == 3) {
                image[newpos + 0] = bmp[bmpos + 2]; //R
                image[newpos + 1] = bmp[bmpos + 1]; //G
                image[newpos + 2] = bmp[bmpos + 0]; //B
                image[newpos + 3] = 255;            //A
            } else {
                image[newpos + 0] = bmp[bmpos + 2]; //R
                image[newpos + 1] = bmp[bmpos + 1]; //G
                image[newpos + 2] = bmp[bmpos + 0]; //B
                image[newpos + 3] = bmp[bmpos + 3]; //A
            }
        }
    return 0;
}

bool image::get_bitmap_bytes(HBITMAP bmp, HPALETTE pallete, std::tuple<unsigned char*, long, IStream*>* bitmap_bytes) {
    PICTDESC pd;
    pd.cbSizeofstruct = sizeof(PICTDESC);
    pd.picType = PICTYPE_BITMAP;
    pd.bmp.hbitmap = bmp;
    pd.bmp.hpal = pallete;

    IPicture* picture;
    HRESULT res = OleCreatePictureIndirect(&pd, IID_IPicture, false, reinterpret_cast<void**>(&picture));

    if (!SUCCEEDED(res)) return false;

    IStream* stream;
    res = CreateStreamOnHGlobal(0, true, &stream);

    if (!SUCCEEDED(res)) {
        picture->Release();
        return false;
    }

    long bytes_streamed;
    res = picture->SaveAsFile(stream, true, &bytes_streamed);

    if (!SUCCEEDED(res)) {
        stream->Release();
        picture->Release();
        return false;
    }

    HGLOBAL mem = 0;
    GetHGlobalFromStream(stream, &mem);
    unsigned char* data = reinterpret_cast<unsigned char*>(GlobalLock(mem));

    *bitmap_bytes = std::make_tuple(data, bytes_streamed, stream);

    GlobalUnlock(mem);
    picture->Release();

    return true;
}

bool image::convert_bmp_to_png(const std::string& path, std::tuple<unsigned char*, long, IStream*> bitmap_bytes) {
    IStream* stream = std::get<2>(bitmap_bytes);

    std::vector<unsigned char> bmp;
    for (int i = 0; i < std::get<1>(bitmap_bytes); i++) {
        bmp.push_back(std::get<0>(bitmap_bytes)[i]);
    }

    std::vector<unsigned char> image;
    unsigned w, h;
    unsigned err = decodeBMP(image, w, h, bmp);

    if (err) {
        stream->Release();
        return false;
    }

    std::vector<unsigned char> png;
    err = lodepng::encode(png, image, w, h);

    if (err) {
        stream->Release();
        return false;
    }

    lodepng::save_file(png, path);

    stream->Release();
    return true;
}