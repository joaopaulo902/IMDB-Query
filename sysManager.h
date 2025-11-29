//
// Created by andri on 29/11/2025.
//

#ifndef SYSMANAGER_H
#define SYSMANAGER_H

#include "sysManager.h"
#include "entities.h"
#include <stdio.h>
#include <stdlib.h>
#define PAGE_SIZE 10

void initialize_system();

void clear_screen();

void print_title_list_header(int currentPage, int totalPages);

void print_info_header();

void print_titles_list(Titles *titles, int totalMovies, int currentPage);

void print_menu_options();

void show_info_page(int totalMovies);

#endif //SYSMANAGER_H
