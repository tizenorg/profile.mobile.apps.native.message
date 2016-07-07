#define RESOURCE_IMAGE( FILE_NAME ) \
   group { \
      name: FILE_NAME; \
      images.image: FILE_NAME COMP; \
      parts { \
         part { name: "image"; \
            description { \
               state: "default" 0.0; \
               image.normal: FILE_NAME; \
               aspect: 1 1; \
               aspect_preference: BOTH; \
            } \
         } \
      } \
   }

#define RESOURCE_IMAGE_COLOR( FILE_NAME, COLOR ) \
   group { \
      name: FILE_NAME; \
      images.image: FILE_NAME COMP; \
      parts { \
         part { name: "image"; \
            description { \
               state: "default" 0.0; \
               color: COLOR; \
               image.normal: FILE_NAME; \
               aspect: 1 1; \
               aspect_preference: BOTH; \
            } \
         } \
      } \
   }

#define RESOURCE_IMAGE_NO_ASPECT( FILE_NAME ) \
   group { \
      name: FILE_NAME; \
      images.image: FILE_NAME COMP; \
      parts { \
         part { name: "image"; \
            description { \
               state: "default" 0.0; \
               image.normal: FILE_NAME; \
            } \
         } \
      } \
   }

#define RESOURCE_IMAGE_LOSSY( FILE_NAME ) \
   group { \
      name: FILE_NAME; \
      images.image: FILE_NAME LOSSY 85; \
      parts { \
         part { name: "image"; \
            description { \
               state: "default" 0.0; \
               image.normal: FILE_NAME; \
               aspect: 1 1; \
               aspect_preference: BOTH; \
            } \
         } \
      } \
   }

#define RESOURCE_OVERLAP_IMAGE( GROUP_NAME, BG_FILE_NAME, BG_COLOR_R, BG_COLOR_G, BG_COLOR_B, BG_COLOR_A, ICON_FILE_NAME, ICON_COLOR_R, ICON_COLOR_G, ICON_COLOR_B, ICON_COLOR_A) \
		group { \
			name: GROUP_NAME; \
			images.image: BG_FILE_NAME COMP; \
			images.image: ICON_FILE_NAME COMP; \
			parts { \
				part { \
					name: "bg"; \
					description { \
						state: "default" 0.0; \
						image.normal: BG_FILE_NAME; \
						color: BG_COLOR_R BG_COLOR_G BG_COLOR_B BG_COLOR_A;\
					} \
				} \
				part { \
					name: FILE_NAME; \
					description { \
						state: "default" 0.0; \
						image.normal: ICON_FILE_NAME; \
						color: ICON_COLOR_R ICON_COLOR_G ICON_COLOR_B ICON_COLOR_A;\
					} \
				} \
			} \
		}

