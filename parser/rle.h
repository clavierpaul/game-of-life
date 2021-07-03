//
// Created by Paul Clavier on 03/07/2021.
//

#ifndef GAMEOFLIFE_RLE_H
#define GAMEOFLIFE_RLE_H

void rle_add_tag(char* tag);
void rle_set_width(int width);
void rle_set_height(int height);
void rle_set_rules(int born, int survive);
void rle_parse(char* file);

#endif //GAMEOFLIFE_RLE_H
