#include <lcom/lcf.h>
#include <math.h>
#include "grafics.h"


unsigned int bppixel;
void* init_adress;
unsigned int x_res;
unsigned y_res;
unsigned color_mode;
unsigned bytes_per_pixel;
unsigned int bytes_per_pixel;
static unsigned int vram_base;  
static unsigned int vram_size; 
vbe_mode_info_t mode_info;
struct reg86 r;  
static void* v_mem;

int vbe_get_mode_inf(uint16_t mode, vbe_mode_info_t *vmi_p){
  uint32_t size=sizeof(vbe_mode_info_t);
  mmap_t adr;
  lm_alloc(size,&adr);
  struct reg86 r;  
  memset(&r,0,sizeof(r)); 
  r.es = PB2BASE(adr.phys);    
  r.di = PB2OFF(adr.phys);            
  r.ax = 0x4F01;          
  r.cx = mode;
  r.intno = 0x10;
  *vmi_p=*(vbe_mode_info_t*)adr.virt;
  lm_free(&adr);
  return 0;
}

int set_vbe_mode(uint16_t mode){
  struct minix_mem_range mr;
  mr.mr_base=0;
  mr.mr_limit=1<<20;
  memset(&r,0,sizeof(r));
  r.ax = 0x4f02;
  r.bx = BIT(14)|mode;
  r.intno = 0x10;

  if( sys_int86(&r) != OK )return 1;
  return 0;
}

int get_mode_info(uint16_t mode){
  vbe_get_mode_info(mode, &mode_info);
  struct minix_mem_range mr;
  x_res=mode_info.XResolution;
  y_res=mode_info.YResolution;
  bppixel=mode_info.BitsPerPixel;
  bytes_per_pixel=(bppixel+7)/8;
  vram_base=(phys_bytes)mode_info.PhysBasePtr;
  vram_size=(x_res*y_res*bytes_per_pixel);
  mr.mr_base=vram_base;
  mr.mr_limit=mr.mr_base+vram_size;
  sys_privctl(SELF,SYS_PRIV_ADD_MEM,&mr);
  v_mem=vm_map_phys(SELF,(void*)mr.mr_base,vram_size);
  color_mode=mode_info.MemoryModel;
  return 0; 
}

void*(vg_init)( uint16_t mode){
   
  
  //if(set_vbe_mode(mode))return NULL;;
  //if(get_mode_info(mode))return NULL;;
  //return v_mem;
  return NULL;
}

int (vg_draw_rectangle)(uint16_t	x, uint16_t  y, uint16_t width, uint16_t height, uint32_t color)
{

  for (uint16_t i = 0; i < height; i++) {
    vg_draw_hline(x, y + i, width, color);
  }

  return 0;
}


int (draw_pixel)(uint16_t x, uint16_t y, uint32_t color){
  if ( y >= y_res || x >= x_res)return 1;
  
    uint8_t bytes_per_pixel =  ceil(bppixel / 8.0);

    if (bytes_per_pixel == 1) *((char*)v_mem + (y*x_res*bytes_per_pixel)+(x*bytes_per_pixel) ) = (unsigned char) color;
    else
    {
      for(int i = 0; i < bytes_per_pixel; i++)
      {
        *((char*)v_mem + (bytes_per_pixel*y*x_resel / 8.0);

    if (bytes_per_pixel == 1) *((char*)v_mem + (y*x_res*bytes_per_pixel)+(x*bytes_per_pixel) ) = (unsigned char) color;
    else
    {
      for(int i = 0; i < bytes_per_pixel; i++)
      {
        *((char*)v_mem + (bytes_per_pixel*y*x_res) + (x*bytes_per_pixel) + i) = (uint8_t)color;
        color=color>>8;
      }
      
    }
        
    return 0;
}

int (vg_draw_hline)(uint16_t	x, uint16_t  y, uint16_t len, uint32_t color)
{
 for (uint16_t j = 0; j < len; j++) draw_pixel((uint16_s)x +j, y, color);
  
  return 0; 
}

 

int draw_xpm(int x ,int y, xpm_map_t xpm){
  uint8_t* sprite; 
  xpm_image_t img;
  sprite=xpm_load(xpm,XPM_5_6_5,&img);
  memset(v_mem,0xff,x_res*y_res*2);
  int bcount=0;
  for (int i=y; i< img.height+y;i++){
)x +j, y, color);
  
  return 0; 
}

 

int draw_xpm(int x ,int y, xpm_map_t xpm){
  uint8_t* sprite; 
  xpm_image_t img;
  sprite=xpm_load(xpm,XPM_5_6_5,&img);
  memset(v_mem,0xff,x_res*y_res*2);
  int bcount=0;
  for (int i=y; i< img.height+y;i++){
