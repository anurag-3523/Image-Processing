#include<stdio.h>
#include<stdlib.h>
#include<inttypes.h>
#pragma pack(push,1)
struct BMP_header {
	uint16_t type;
	uint32_t size;
	uint16_t reserve1;
	uint16_t reserve2;
	uint32_t offset;

};
#pragma pack(pop)

#pragma pack(push,1)
struct DIB_header{
	uint32_t header_size;
    uint32_t width;
    uint32_t height;
    uint16_t colorplanes;
    uint16_t bitperpixel;
    uint32_t compression;
    uint32_t image_size;
    uint32_t PPM_X;
    uint32_t PPM_Y;
    uint32_t color_used;
    uint32_t color_imported;
};
#pragma pack(pop)

unsigned char *load(char *filename,struct DIB_header *infofile){
   
    FILE *fp;
    
    struct BMP_header bitheader;
    unsigned char *bitmapdata;
    int index;
    fp = fopen(filename,"rb");
    
	if(fp==NULL){
	    return NULL;
    }
   
    

    fread(&bitheader, sizeof(struct BMP_header), 1, fp);
    if(bitheader.type!=0x4D42){
        fclose(fp);
        return NULL;   
    }
    
    fread(infofile,sizeof(struct DIB_header),1,fp);
	fseek(fp,bitheader.offset,SEEK_SET);
    bitmapdata=(unsigned char*)malloc(infofile->image_size);
    fread(bitmapdata,infofile->image_size,1,fp);

    for(index=0; index<infofile->image_size; index+=3)
    {
    
        //bitmapdata[index]=0; //blue  
        //bitmapdata[index+1]=0; //green
        //bitmapdata[index+2]=255; //red
        
         if(bitmapdata[index]==255 && bitmapdata[index+1]==0 && bitmapdata[index+2]==255)
        {
            bitmapdata[index]=0; //blue  
            bitmapdata[index+1]=0; //green
            bitmapdata[index+2]=255; //red
        }
    }
   

    fclose(fp);
    return bitmapdata;
}

int update(char *filename, unsigned char *bitmapdata, struct DIB_header *infofile){
    FILE *fp;
    struct BMP_header bitheader;
    int index;
    fp = fopen(filename,"rb+");
	if(fp==NULL)
	    return -1;

    fread(&bitheader, sizeof(struct BMP_header), 1, fp);
    if(bitheader.type!=0x4D42){
        fclose(fp);
        return -1;  
    }
    
	fseek(fp,bitheader.offset,SEEK_SET);
    fwrite(bitmapdata,infofile->image_size,1,fp);
    
    fclose(fp);
    return 0;


}

int main() {
    struct DIB_header infoheader;
    
    unsigned char *bitmapdata;
    bitmapdata=load("red.bmp",&infoheader);
    update("red.bmp",bitmapdata,&infoheader);
	
	return 0;
}