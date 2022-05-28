#include <iostream>

#include <vg/vg.h>

#include "compositor.h"
#include "backing_surface.h"
#include "backing_layer.h"

namespace attacus {

void BackingLayer::Create() {
    width_ = surface_->width();
    height_ = surface_->height();
    //image_ = createImage(vg_, vg::ImageFlags::Filter_NearestUV, surface_->texture_);
    //imgPaint_ = createImagePattern(vg_, 0, 0, width_, height_, 0.0f, image_);
    imgPaint_ = createImagePattern(vg_, 0, 0, width_, height_, 0.0f, surface_->image_);
}

void BackingLayer::Destroy() {
    //vg::destroyImage(vg_, image_);
    comp().FreeSurface(*surface_);
    Component::Destroy();
}

void BackingLayer::Draw() {
    vg::beginPath(vg_);
    vg::rect(vg_, 0, 0, width_, height_);
    vg::fillPath(vg_, imgPaint_, vg::color4f(1.0f, 1.0f, 1.0f, 1.0f), vg::FillFlags::Convex);
}

} // namespace attacus