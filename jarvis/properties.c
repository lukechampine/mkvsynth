#include "properties.h"

int getDepth(MkvsynthMetaData *metaData) {
	switch(metaData->colorspace) {
		case 1:
			return 16;
		case 2:
			return 8;
		case 3:
			return 16;
		case 4:
			return 8;
	}
}
