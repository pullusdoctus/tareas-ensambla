#ifndef PROVINCIAS_H
#define PROVINCIAS_H

#include <stddef.h>

typedef struct province_t province_t;
typedef struct county_t county_t;
typedef struct district_t district_t;

struct district_t {
  int id;
  char* name;
  int county_id;
};

struct county_t {
  int id;
  char* name;
  int province_id;
  struct district_t* districts;
  size_t district_count;
};

struct province_t {
  int id;
  char* name;
  struct county_t* counties;
  size_t county_count;
};

struct location_data_t {
  struct province_t* provinces;
  size_t province_count;
};

struct selected_address_t{
    int province_id;
    int county_id;
    int district_id;
    char* province_name;
    char* county_name;
    char* district_name;
};

struct province_t* find_province_by_id(const struct location_data_t* data, int province_id);
struct county_t* find_county_by_id(const struct province_t* province, int county_id);
struct district_t* find_district_by_id(const struct county_t* county, int district_id);

void free_location_data(struct location_data_t* data);
struct location_data_t* create_location_data(void);
void destroy_location_data(struct location_data_t* data);

char* create_string_copy(const char* src);

#endif
