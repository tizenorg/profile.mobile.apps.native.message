
#define PADDING_LEFT(param_padding_name, param_padding_size) \
	part \
	{ \
		name: param_padding_name; \
		type: SPACER; \
		scale: 1; \
		mouse_events:  0;\
		description \
		{ \
			state: "default" 0.0; \
			min: param_padding_size 0; \
			fixed: 1 0; \
			rel1{relative: 0.0 0.0;} \
			rel2{relative: 0.0 1.0;} \
			align: 0.0 0.0; \
		} \
	}
#define PADDING_TOP(param_padding_name, param_padding_size) \
	part \
	{ \
		name: param_padding_name; \
		type: SPACER; \
		scale: 1; \
		mouse_events:  0;\
		description \
		{ \
			state: "default" 0.0; \
			min: 0 param_padding_size ; \
			fixed: 0 1; \
			rel1{relative: 0.0 0.0;} \
			rel2{relative: 1.0 0.0;} \
			align: 0.0 0.0; \
		} \
	}
#define PADDING_BOTTOM(param_padding_name, param_padding_size) \
	part \
	{ \
		name: param_padding_name; \
		type: SPACER; \
		scale: 1; \
		mouse_events:  0;\
		description \
		{ \
			state: "default" 0.0; \
			min: 0 param_padding_size; \
			fixed: 0 1; \
			rel1{relative: 0.0 1.0;} \
			rel2{relative: 1.0 1.0;} \
			align: 0.0 1.0; \
		} \
	}

#define PADDING_RIGHT(param_padding_name, param_padding_size) \
	part \
	{ \
		name: param_padding_name; \
		type: SPACER; \
		scale: 1; \
		mouse_events:  0;\
		description \
		{ \
			state: "default" 0.0; \
			min: param_padding_size 0; \
			fixed: 1 0; \
			rel1{relative: 1.0 0.0;} \
			rel2{relative: 1.0 1.0;} \
			align: 1.0 0.0; \
		} \
	}
#define PART_DISCLIP \
	part \
	{ \
		name: "disclip"; \
		type: RECT; \
		description \
		{ \
			state: "default" 0.0; \
		} \
		description \
		{ \
			state: "disabled" 0.0; \
			inherit: "default" 0.0; \
			color: 255 255 255 64; \
		} \
	}

