#include <ruby.h>
#include <stdint.h>
#include <x264.h>

VALUE RB_x264_ns; // module
VALUE RB_x264_t;
VALUE RB_x264_level_t;
VALUE RB_x264_image_t;
VALUE RB_x264_picture_t;
VALUE RB_x264_nal_t;

typedef struct _RBX264 {
  x264_t *x264;
  x264_param_t param;
} RBX264;

#define RB_X264_SET_INT(type, attr, val) \
  type *ptr; \
  Data_Get_Struct(self,type,ptr); \
  ptr->attr = NUM2INT(attr); \
  return self;

#define RB_X264_GET_INT(type, attr) \
  type *ptr; \
  Data_Get_Struct(self,type,ptr); \
  return INT2NUM(ptr->attr);

#define RB_X264_SET_INT_S2(type, a1, a2, val) \
  type *ptr; \
  Data_Get_Struct(self,type,ptr); \
  ptr->a1.a2 = NUM2INT(attr); \
  return self;

#define RB_X264_GET_INT_S2(type, a1, a2) \
  type *ptr; \
  Data_Get_Struct(self,type,ptr); \
  return INT2NUM(ptr->a1.a2);

#define RBX_PARAM_RC_ISETTER(attr) \
static VALUE rb_x264_rc_##attr##_set(VALUE self, VALUE val) { \
  RBX264 *rbx = NULL; \
  Data_Get_Struct(self, RBX264, rbx); \
  rbx->param.rc.attr = NUM2INT(val); \
  return self; \
}
#define RBX_PARAM_RC_IGETTER(attr) \
static VALUE rb_x264_rc_##attr##_get(VALUE self) {\
  RBX264 *rbx = NULL; \
  Data_Get_Struct(self, RBX264, rbx); \
  return INT2NUM(rbx->param.rc.attr); \
}
#define RBX_PARAM_RC_IGET_ISET(attr) \
  RBX_PARAM_RC_ISETTER(attr) \
  RBX_PARAM_RC_IGETTER(attr) \

#define RB_GETTER_SETTER(attr,name) \
  rb_define_method(RB_x264_t, name"=", rb_x264_rc_##attr##_set, 1); \
  rb_define_method(RB_x264_t, name, rb_x264_rc_##attr##_get, 0);

RBX_PARAM_RC_IGET_ISET(i_bitrate);

RBX_PARAM_RC_IGET_ISET(i_rc_method);

/*
static VALUE rb_x264_rc_bitrate_set(VALUE self, VALUE val)
{
  RBX264 *rbx = NULL;
  Data_Get_Struct(self, RBX264, rbx);
  rbx->param.rc.i_bitrate = NUM2INT(val);
  return self;
}
static VALUE rb_x264_rc_bitrate_get(VALUE self)
{
  RBX264 *rbx = NULL;
  Data_Get_Struct(self, RBX264, rbx);
  return INT2NUM(rbx->param.rc.i_bitrate);
}*/

static void rb_x264_free(RBX264 *rbx)
{
  if( rbx ) {
    x264_encoder_close(rbx->x264);
    free(rbx);
  }
}

/*
static VALUE rb_x264_x264_alloc(VALUE klass)
{
  VALUE rbxo;
  RBX264 *rbx = ALLOC(RBX264);
  x264_param_default( &(rbx->param) );
  // initial defaults
  rbx->param.i_width = 32;
  rbx->param.i_height = 32;
  rbx->param.rc.i_bitrate = 10;
  rbx->param.rc.i_rc_method = X264_RC_CQP;
  rbx->x264 = x264_encoder_open( &(rbx->param) );

  rbxo = Data_Wrap_Struct(klass, NULL, rb_x264_free, rbx);

  return rbxo;
}*/

static VALUE rb_x264_config(VALUE klass)
{
  VALUE rbxo;
  RBX264 *rbx = ALLOC(RBX264);

  x264_param_default( &(rbx->param) );

  // initial safe defaults
  rbx->param.i_width = 32;
  rbx->param.i_height = 32;
  rbx->param.rc.i_bitrate = 10;
  rbx->param.rc.i_rc_method = X264_RC_CQP;

  // wrap the data in ruby object
  rbxo = Data_Wrap_Struct(klass, NULL, rb_x264_free, rbx);

  // if the block is given pass it the new encoder
  if (rb_block_given_p()) {
    rb_yield(rbxo);
  }

  // create the encoder
  rbx->x264 = x264_encoder_open( &(rbx->param) );

  return rbxo;
}
  

/*static VALUE rb_x264_encoder_reconfig(VALUE self, VALUE options)
{
  x264_t *x264;
  Data_Get_Struct(self, x264_t, x264);
  return self;
}*/

void Init_x264()
{
  RB_x264_ns        = rb_define_module("X264");  

  /* define some consts */
  rb_define_const(RB_x264_ns, "RC_NONE", LONG2NUM(X264_RC_NONE));
  rb_define_const(RB_x264_ns, "RC_CQP", LONG2NUM(X264_RC_CQP));
  rb_define_const(RB_x264_ns, "RC_CRF", LONG2NUM(X264_RC_CRF));
  rb_define_const(RB_x264_ns, "RC_ABR", LONG2NUM(X264_RC_ABR));

  /* define the class's */
  RB_x264_t    = rb_define_class_under(RB_x264_ns, "Encoder", rb_cObject);
  RB_x264_level_t   = rb_define_class_under(RB_x264_ns, "Level", rb_cObject);
  RB_x264_image_t   = rb_define_class_under(RB_x264_ns, "Image", rb_cObject);
  RB_x264_picture_t = rb_define_class_under(RB_x264_ns, "Picture", rb_cObject);
  RB_x264_nal_t     = rb_define_class_under(RB_x264_ns, "Nal", rb_cObject);

  /* define X264::X264 instance methods */
 // rb_define_alloc_func(RB_x264_t, rb_x264_x264_alloc);

  rb_define_singleton_method(RB_x264_t, "config", rb_x264_config, 0);
  RB_GETTER_SETTER(i_bitrate,"rc_bitrate");
  RB_GETTER_SETTER(i_rc_method,"rc_method");
//  rb_define_method(RB_x264_t, "rc_bitrate=", rb_x264_rc_bitrate_set, 1);
//  rb_define_method(RB_x264_t, "rc_bitrate", rb_x264_rc_bitrate_get, 0);

//  rb_define_singleton_method(RB_x264_t, "close", rb_x264_x264_close, 0);
}
