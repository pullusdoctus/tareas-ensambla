#include "../inc/Location.h"
#include <stdlib.h>
#include <string.h>

province_t* find_province_by_id(const struct location_data_t* data, int province_id) {
  if (!data || !data->provinces) {
    return NULL;
  }
  for (size_t i = 0; i < data->province_count; i++) {
    if (data->provinces[i].id == province_id) {
      return &data->provinces[i];
    }
  }
  return NULL;
}

county_t* find_county_by_id(const struct province_t* province, int county_id) {
  if (!province || !province->counties) {
    return NULL;
  }
  for (size_t i = 0; i < province->county_count; i++) {
    if (province->counties[i].id == county_id) {
      return &province->counties[i];
    }
  }
  return NULL;
}

district_t* find_district_by_id(const struct county_t* county, int district_id) {
  if (!county || !county->districts) {
    return NULL;
  }
  for (size_t i = 0; i < county->district_count; i++) {
    if (county->districts[i].id == district_id) {
      return &county->districts[i];
    }
  }
  return NULL;
}

struct county_t* find_county_by_id_global(const struct location_data_t* data, int county_id) {
  if (!data || !data->provinces) {
    return NULL;
  }
  for (size_t i = 0; i < data->province_count; i++) {
    struct county_t* county = find_county_by_id(&data->provinces[i], county_id);
    if (county) {
      return county;
    }
  }
  return NULL;
}

struct district_t* find_district_by_id_global(const struct location_data_t* data, int district_id) {
  if (!data || !data->provinces) {
    return NULL;
  }
  for (size_t i = 0; i < data->province_count; i++) {
    const province_t* province = &data->provinces[i];
    if (!province->counties) continue;
    for (size_t j = 0; j < province->county_count; j++) {
      struct district_t* district = find_district_by_id(&province->counties[j], district_id);
      if (district) {
        return district;
      }
    }
  }
  return NULL;
}

struct province_t* get_county_province(const struct location_data_t* data, const struct county_t* county) {
  if (!data || !county) {
    return NULL;
  }
  return find_province_by_id(data, county->province_id);
}

// Helper function to get parent county of a district
county_t* get_district_county(const struct location_data_t* data, const struct district_t* district) {
  if (!data || !district) {
    return NULL;
  }
  // Find the county that contains this district
  for (size_t i = 0; i < data->province_count; i++) {
      const province_t* province = &data->provinces[i];
      if (!province->counties) continue;
      for (size_t j = 0; j < province->county_count; j++) {
      if (province->counties[j].id == district->county_id) {
        return &province->counties[j];
      }
    }
  }
  return NULL;
}

// Memory management functions
void free_district_array(struct district_t* districts, size_t count) {
  if (!districts) return;
  for (size_t i = 0; i < count; i++) {
    free(districts[i].name);
  }
  free(districts);
}

void free_county_array(struct county_t* counties, size_t count) {
  if (!counties) return;
  for (size_t i = 0; i < count; i++) {
    free(counties[i].name);
    free_district_array(counties[i].districts, counties[i].district_count);
  }
  free(counties);
}

void free_province_array(struct province_t* provinces, size_t count) {
  if (!provinces) return;
  for (size_t i = 0; i < count; i++) {
    free(provinces[i].name);
    free_county_array(provinces[i].counties, provinces[i].county_count);
  }
  free(provinces);
}

void free_location_data(struct location_data_t* data){
  if (!data) return;
  free_province_array(data->provinces, data->province_count);
  data->provinces = NULL;
  data->province_count = 0;
}

struct location_data_t* create_location_data(void) {
  struct location_data_t* data = malloc(sizeof(struct location_data_t));
  if (data) {
    data->provinces = NULL;
    data->province_count = 0;
  }
  return data;
}

void destroy_location_data(struct location_data_t* data) {
  if (data) {
    free_location_data(data);
    free(data);
  }
}

char* create_string_copy(const char* src) {
  if (!src) return NULL;
  size_t len = strlen(src) + 1;
  char* copy = malloc(len);
  if (copy) {
    strcpy(copy, src);
  }
  return copy;
}
