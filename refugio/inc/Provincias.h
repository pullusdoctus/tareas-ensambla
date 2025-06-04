#ifndef PROVINCIAS_H
#define PROVINCIAS_H

struct Provincia {
  int id;
  char* value;
};

struct Canton {
  int id;
  int parent;
  char* value;
};

#endif
