#include <ruby.h>
#include <stdint.h>
#include <x264.h>

VALUE RB_x264_ns; // module
VALUE RB_x264_encoder_t;
VALUE RB_x264_level_t;
VALUE RB_x264_image_t;
VALUE RB_x264_picture_t;
VALUE RB_x264_nal_t;

static void rb_x264_encoder_close(x264_t *x264)
{
  
}

static VALUE rb_x264_encoder_new(VALUE self, VALUE options)
{
  VALUE x264_object;
  x264_param_t param;
  x264_param_default( &param );
  x264_t *x264 = x264_encoder_open( &param );

  x264_object = Data_Wrap_Struct(RB_x264_encoder_t, NULL, x264_encoder_close, x264);

  return x264_object;
}
static VALUE rb_x264_encoder_reconfig(VALUE self, VALUE options)
{
  return self;
}

void Init_x264()
{
  RB_x264_ns        = rb_define_module("X264");  

  /* define the class's */
  RB_x264_encoder_t = rb_define_class_under(RB_x264_ns, "Encoder", rb_cObject);
  RB_x264_level_t   = rb_define_class_under(RB_x264_ns, "Level", rb_cObject);
  RB_x264_image_t   = rb_define_class_under(RB_x264_ns, "Image", rb_cObject);
  RB_x264_picture_t = rb_define_class_under(RB_x264_ns, "Picture", rb_cObject);
  RB_x264_nal_t     = rb_define_class_under(RB_x264_ns, "Nal", rb_cObject);

  /* define X264::Encoder instance methods */
  rb_define_singleton_method(RB_x264_encoder_t, "new", rb_x264_encoder_new, 1);
  rb_define_singleton_method(RB_x264_encoder_t, "reconfig", rb_x264_encoder_reconfig, 1);
//  rb_define_singleton_method(RB_x264_encoder_t, "close", rb_x264_encoder_close, 0);
}
