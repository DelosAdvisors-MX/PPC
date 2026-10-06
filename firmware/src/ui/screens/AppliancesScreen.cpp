#include "AppliancesScreen.h"

#include <stdio.h>

#include "../../board/Metrics.h"
#include "../Fonts.h"
#include "../Format.h"
#include "../Theme.h"

namespace ui {
namespace {

constexpr int32_t GLYPH = 26;
constexpr int32_t SHARE_W = 46;
constexpr int32_t BAR_H = 14;

}  // namespace

void AppliancesScreen::build(lv_obj_t* root) {
  headline_ = lv_label_create(root);
  lv_obj_set_pos(headline_, metrics::PADDING, metrics::PADDING);
  lv_obj_set_style_text_font(headline_, fonts::title(), 0);
  lv_obj_set_style_text_color(headline_, lv_color_hex(theme::INK), 0);

  intro_ = lv_label_create(root);
  lv_obj_set_pos(intro_, metrics::PADDING, metrics::PADDING + metrics::px(28));
  lv_obj_set_width(intro_, metrics::CONTENT_WIDTH);
  lv_obj_set_style_text_font(intro_, fonts::micro(), 0);
  lv_obj_set_style_text_color(intro_, lv_color_hex(theme::CHIP_INK), 0);

  // Five rows, spread through the space the heading leaves.
  const int32_t first_y = metrics::PADDING + metrics::px(54);
  const int32_t pitch = (metrics::HEIGHT - metrics::PADDING - first_y) / MAX_ROWS;
  const int32_t text_x = metrics::PADDING + metrics::px(GLYPH + 10);
  const int32_t bar_w =
      metrics::CONTENT_WIDTH - metrics::px(GLYPH + 10) - metrics::px(SHARE_W + 10);

  for (size_t i = 0; i < MAX_ROWS; i++) {
    Row& row = rows_[i];
    const int32_t y = first_y + static_cast<int32_t>(i) * pitch;

    row.glyph.attach(root, metrics::PADDING, y + metrics::px(4), metrics::px(GLYPH),
                     metrics::px(GLYPH));

    row.name = lv_label_create(root);
    lv_obj_set_pos(row.name, text_x, y);
    lv_obj_set_style_text_font(row.name, fonts::body(), 0);
    lv_obj_set_style_text_color(row.name, lv_color_hex(theme::INK), 0);

    row.track = lv_obj_create(root);
    lv_obj_remove_style_all(row.track);
    lv_obj_set_pos(row.track, text_x, y + metrics::px(24));
    lv_obj_set_size(row.track, bar_w, metrics::px(BAR_H));
    lv_obj_set_style_radius(row.track, metrics::px(7), 0);
    lv_obj_set_style_bg_color(row.track, lv_color_hex(theme::WASH), 0);
    lv_obj_set_style_bg_opa(row.track, LV_OPA_COVER, 0);
    lv_obj_remove_flag(row.track, LV_OBJ_FLAG_SCROLLABLE);

    row.fill = lv_obj_create(row.track);
    lv_obj_remove_style_all(row.fill);
    lv_obj_set_pos(row.fill, 0, 0);
    lv_obj_set_height(row.fill, metrics::px(BAR_H));
    lv_obj_set_style_radius(row.fill, metrics::px(7), 0);
    lv_obj_set_style_bg_color(row.fill, lv_color_hex(theme::ORANGE), 0);
    lv_obj_set_style_bg_opa(row.fill, LV_OPA_COVER, 0);

    row.share = lv_label_create(root);
    lv_obj_set_pos(row.share, metrics::WIDTH - metrics::PADDING - metrics::px(SHARE_W),
                   y + metrics::px(20));
    lv_obj_set_width(row.share, metrics::px(SHARE_W));
    lv_obj_set_style_text_align(row.share, LV_TEXT_ALIGN_RIGHT, 0);
    lv_obj_set_style_text_font(row.share, fonts::micro(), 0);
    lv_obj_set_style_text_color(row.share, lv_color_hex(theme::INK), 0);
  }
}

void AppliancesScreen::update(const MeterSnapshot& snapshot) {
  if (root_ == nullptr) return;
  const AppliancesData& data = snapshot.appliances;

  lv_label_set_text(headline_, data.headline);
  lv_label_set_text(intro_, data.intro);

  // Bars are relative to the heaviest appliance; the figure on the right is
  // that appliance's share of the whole year, which is why the two disagree.
  float heaviest = 1.0f;
  for (size_t i = 0; i < data.count; i++) {
    if (data.items[i].kwh > heaviest) heaviest = data.items[i].kwh;
  }

  char label[48];
  char figure[16];

  for (size_t i = 0; i < MAX_ROWS; i++) {
    Row& row = rows_[i];
    const bool used = i < data.count;
    lv_obj_set_style_opa(row.name, used ? LV_OPA_COVER : LV_OPA_TRANSP, 0);
    lv_obj_set_style_opa(row.track, used ? LV_OPA_COVER : LV_OPA_TRANSP, 0);
    lv_obj_set_style_opa(row.share, used ? LV_OPA_COVER : LV_OPA_TRANSP, 0);
    if (!used) continue;

    const Appliance& item = data.items[i];
    row.glyph.set_icon(item.icon);

    snprintf(label, sizeof(label), "%s: %s", item.name,
             fmt::kwh(figure, sizeof(figure), item.kwh));
    lv_label_set_text(row.name, label);

    lv_obj_set_width(row.fill,
                     static_cast<int32_t>(lv_obj_get_width(row.track) * (item.kwh / heaviest)));

    snprintf(label, sizeof(label), "%s%%", fmt::decimal(figure, sizeof(figure), item.share));
    lv_label_set_text(row.share, label);
  }
}

}  // namespace ui
