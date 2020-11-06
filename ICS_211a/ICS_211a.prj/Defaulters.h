// Defaulters -- Person that do not check out, fixing them


#pragma once

struct AttdDsc;


class Defaulters {

public:

  Defaulters() { }
 ~Defaulters() { }

  void operator() ();

private:

  void updateRoster(AttdDsc* dsc);
  void addRoster(AttdDsc* dsc);
  };

