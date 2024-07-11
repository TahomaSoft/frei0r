  #include <frei0r.hpp>
  
  typedef struct {
    int16_t w, h;
    uint8_t bpp;
    uint32_t size;
  } ScreenGeometry;
  
  class MyExample: public frei0r::filter {
  public:
    MyExample(int wdt, int hgt);
    ~MyExample();
    virtual void update();
  private:
    ScreenGeometry geo;
    void _init(int wdt, int hgt);
  }
  
  MyExample::MyExample() { /* constructor */ }
  MyExample::~MyExample() { /* destructor */ }
  
  void MyExample::_init(int wdt, int hgt) {
    geo.w = wdt;
    geo.h = hgt;
    geo.bpp = 32; // this filter works only in RGBA 32bit
    geo.size = geo.w*geo.h*(geo.bpp/8); // calculate the size in bytes
  }
  
  void MyExample::update() {
    // we get video input via buffer pointer (void*)in 
    uint32_t *src = (uint32_t*)in;
    // and we give video output via buffer pointer (void*)out
    uint32_t *dst = (uint32_t*)out;
    // this example here does just a copy of input to output
    memcpy(dst, src, geo.size);
  }
    
  frei0r::construct<MyExample>
          plugin("MyExample", "short and simple description for my example",
                 "Who did it", 1, 0);

