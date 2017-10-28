//  CS 430 Project 2 Raycasting
//  Junzhe Wang 
//  jw2323
//  jw2323@nau.edu
//  2017/10/11

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include <errno.h>
#define CAMERA 1
#define SPHERE 2
#define PLANE 3


typedef struct {   // create a Object to store all the properties of sphere/plane
    int kind;
    //camera
    float width;
    float height;    // declare all the variable
    
    //sphere
    float position[3];
    float color[3];
    int radius;
    
    //plane
    float normal[3];
    
} Object;

Object objects[128];   // define all variables and functions ahead
int object_count;
float camera_width ;
float camera_height ;

void read_csv(char *, Object*);

void parse_type(Object*,int,char*);

void parse_field(Object*,int,char *);

float plane_intersection(float[] ,float[],Object obj);

float sphere_intersection(float[] ,float[],Object obj);
float dot_product(float[],float[]);

float vector_length(float[]);

float* vector_add(float[],float[]);
float* vector_substract(float[],float[]);
float* vector_scale(float[], float);
float point_distance(float[], float[]);
void render(int,int,char *);
int writeP3(FILE*, unsigned char*, int, int);
int writeHeader(FILE *, int, int, int, int);



// main method, used to check argument errors, when the input argument are wrong, return error messages.
int main(int argc, char *argv[])
{
    if(argc != 5){
        printf("Error: Please input the usage pattern like : raycast width height input.csv output.ppm.\n");
        exit(1);
    }
	if (atoi(argv[1]) <= 0 || atoi(argv[2]) <= 0) {
        fprintf(stderr, "Error: main: width and height parameters must be > 0\n");
        exit(1);
    }
	const char *input = argv[3];                                 // check the error of passing arguments. for example, if the argument is empty.
    if (input == NULL) {
        fprintf(stderr, "Error: main: Failed to open input file '%s'\n", argv[3]);
        exit(1);
    }
    int width = atoi(argv[1]);
    int height = atoi(argv[2]);

    printf("width:%d;height:%d\n",width,height);
    
    read_csv(argv[3], objects);   // call the read file function

	printf("camera: \n");
	printf("width %f \n",objects[0].width);
	printf("height %f \n",objects[0].height);
	printf("type %d \n",objects[0].kind);        // test the camera is correctly loaded.
	

	printf ("sphere: \n");
	
	printf("%f \n",objects[1].color[0]);
	printf("%f \n",objects[1].color[1]);
	printf("%f \n",objects[1].color[2]);
	
	printf ("position: \n");
	printf("%f \n",objects[1].position[0]);
	printf("%f \n",objects[1].position[1]);
	printf("%f \n",objects[1].position[2]);
	printf ("radius: \n");
	printf("%d \n",objects[1].radius);
	
	
	printf ("plane: \n");
	
	printf("%f \n",objects[2].color[0]);
	printf("%f \n",objects[2].color[1]);
	printf("%f \n",objects[2].color[2]);
	
	printf ("position: \n");
	printf("%f \n",objects[2].position[0]);
	printf("%f \n",objects[2].position[1]);
	printf("%f \n",objects[2].position[2]);
	printf ("normal: \n");
	printf("%f \n",objects[2].normal[0]);
	printf("%f \n",objects[2].normal[1]);
	printf("%f \n",objects[2].normal[2]);
	
	camera_height=objects[0].height;
	camera_width=objects[0].width;
	
	
    render(width,height,argv[4]);
    /*
    float Ro[3] = {0,0,0};
    float Rd[3] = {0,1,2};
    Object obj = objects[2];

    float t = plane_intersection(Ro,Rd,obj);
    float q = sphere_intersection(Ro,Rd, objects[1]);
    
    printf("%f\n",t);
    */
    
    return 0;
}









// below are the parsing method


char next_c(FILE* csv) {   // define a function to get next character
    char c = fgetc(csv);

    return c;
}


char* next_string(FILE* csv) {    // define a function that can grab a string. 
    char buffer[129];
    char c = next_c(csv);
    int i = 0;
    while ((c != ',' )&& (c!=':')) {   // as long as c is not EOF or comma, the string is not ended.
		if (c ==EOF){
			break;
		}
        if (i >= 128) {
            fprintf(stderr, "Error: Strings longer than 128 characters in length are not supported.\n");
            exit(1);
        }
        if (c == '\\') {
            fprintf(stderr, "Error: Strings with escape codes are not supported.\n");
            exit(1);
        }
        buffer[i] = c;
        i =i+ 1;
        c = next_c(csv);
		buffer[i]='\0';
    }
	ungetc(c,csv);  
	//printf("%s \n" , strdup(buffer));	// unget the one more character. 
	
    return strdup(buffer);
}



char* last_string(FILE* csv) {    // define a function that can grab a string. 
    char buffer[129];
    char c = next_c(csv);
    int i = 0;
    while ((c != ',' )&& (c!=':')) {   // as long as c is not EOF or comma, the string is not ended.
		if (c ==EOF){
			break;
		}
        if (i >= 128) {
            fprintf(stderr, "Error: Strings longer than 128 characters in length are not supported.\n");
            exit(1);
        }
        if (c == '\\') {
            fprintf(stderr, "Error: Strings with escape codes are not supported.\n");
            exit(1);
        }
        buffer[i] = c;
        i =i+ 1;
        c = next_c(csv);
		
    }
	buffer[i-1]='\0';
	ungetc(c,csv);  
	//printf("%s \n" , strdup(buffer));	// unget the one more character. 
	
    return strdup(buffer);
}






double next_number(FILE* csv) {    // define a function to grab numbers.
    double value;
    int res = fscanf(csv, "%lf", &value);
    if (res == EOF) {
        fprintf(stderr, "Error: Expected a number but found EOF.");
        exit(1);
    }
    // printf("next_number: %lf\n", value);
    return value;
}











void skipWS(FILE* csv){
	char c = next_c(csv);
	while (isspace(c)){
		c = next_c(csv);
	}
	ungetc(c,csv);
	// check and skip white space in the csv file
}

void lookfor(FILE* csv, char d){    // locate the wanted character and move the pointer to it.
	while(!next_c(csv)==d){
				char c = next_c(csv);
			}
}

void read_csv(char *input_file, Object* objects)
{
    char buffer[1024];
    FILE *csv = fopen(input_file,"r");
    if(csv == NULL)
    {
        printf("ERROR: file not open!");
		exit(1);
    }
    int line = 0; 
	int counter= 0;

	skipWS(csv);// skip white space
	
	char firstLetter = next_c(csv);
	if (firstLetter!='c'){
		printf("Error: CSV file shouold start with word camera.");
		exit(1);
	}
	ungetc(firstLetter,csv);
	// Check if the file start with camera object.
	
	
    while(1){
		char *type = next_string(csv);

		if (strcmp(type, "camera")==0){
			(objects+counter)->kind=CAMERA;
		}
		if (strcmp(type, "sphere")==0){
			(objects+counter)->kind=SPHERE;
		}
		if (strcmp(type,"plane")==0){
			(objects+counter)->kind=PLANE;
		}
		lookfor(csv,',');
		skipWS(csv);
		
		// reading the type
		
		
		
		
		
		char *wid = next_string(csv);
		if (strcmp(wid,"width")==0){
			skipWS(csv);
			lookfor(csv,':');
			skipWS(csv);
			float width = next_number(csv);
			
			//printf("%f \n" ,width);
			
			
			(objects+counter)->width=width;			
			lookfor(csv,',');
			
			
			skipWS(csv);
			char *tall = next_string(csv);
			if (strcmp(tall,"height")==0){
				lookfor(csv,':');
				skipWS(csv);
				float hei = next_number(csv);
				
				//printf("%f \n",hei);
				
				(objects+counter)->height=hei;

						
			}
					
		}
			
		if(strcmp(wid,"color")==0){
			lookfor(csv,'[');
			skipWS(csv);
			
			next_c(csv);
			(objects+counter)->color[0]=atof(next_string(csv));
			skipWS(csv);
			lookfor(csv,',');
			skipWS(csv);
			(objects+counter)->color[1]=atof(next_string(csv));
			lookfor(csv,',');
			skipWS(csv);
			(objects+counter)->color[2]=atof(last_string(csv));
			skipWS(csv);
			lookfor(csv,',');
			skipWS(csv);
			
			char *t = next_string(csv);

			if (strcmp(t,"position")==0){
				
				lookfor(csv,'[');
				skipWS(csv);
				next_c(csv);
				(objects+counter)->position[0]=atof(next_string(csv));
				
				lookfor(csv,',');
				skipWS(csv);
				(objects+counter)->position[1]=atof(next_string(csv));
				
				lookfor(csv,',');
				skipWS(csv);
				(objects+counter)->position[2]=atof(last_string(csv));
			}else {
				printf("Error: can't find the position");
				exit(1);
			}
			skipWS(csv);
			lookfor(csv,',');
			skipWS(csv);
			char *c = next_string(csv);
			if (strcmp(c,"radius")==0){
				printf("yes \n");
				skipWS(csv);
				lookfor(csv,':');
				skipWS(csv);
				(objects+counter)->radius=next_number(csv);
				
				
			}
			if (strcmp(c,"normal")==0){
				skipWS(csv);
				lookfor(csv,'[');
				skipWS(csv);
				next_c(csv);
				(objects+counter)->normal[0]=atof(next_string(csv));
				skipWS(csv);
				lookfor(csv,'[');
				skipWS(csv);
				(objects+counter)->normal[1]=atof(next_string(csv));
				skipWS(csv);
				lookfor(csv,'[');
				skipWS(csv);
				(objects+counter)->normal[2]=atof(last_string(csv));
				
			}
		
		}
		skipWS(csv);
		char g = next_c(csv);
		if (g==EOF){
			break;
		}
		skipWS(csv);
		ungetc(g,csv);
				
		counter= counter+1;
	}
    fclose(csv);
	object_count= counter+1;

}





float *raycast(float Ro[3], float Rd[3]){
    
    Object closest_obj;
    float closest_t = 99999.99;
    for(int i = 0; i < object_count; i ++) {
        
        Object obj = objects[i];
        
        float t;
        if(obj.kind == SPHERE){
            t = sphere_intersection(Ro, Rd, obj);
        }
        if(obj.kind == PLANE){
            t = plane_intersection(Ro,Rd,obj);
        }
        if(t < closest_t){
            closest_t = t;
            closest_obj = obj;
        }
        
    }
    
    static float color[3];
    color[0] = closest_obj.color[0];
    color[1] = closest_obj.color[1];
    color[2] = closest_obj.color[2];
    //TODO: closest_obj = NULL?
    
    return color;
}


void render(int width,int height,char *output_file) {
    
    float pixheight = camera_height / height;
    float pixwidth = camera_width / width;
    //Pixel* pixels = malloc(sizeof(float)*3*width*height);

    
    /*
    Pixel* pixels = malloc(...);
    double* Pij = malloc(sizeof(double)*3);
    */
    float Pij[3];
    unsigned char img[width*height*3];
    int temp[width*height*3];
    int ptr = 0;
    
    for(int i = 0; i < width; i ++){
        Pij[1] = 0 -camera_height/2+pixheight*(i+0.5);//ycoordofrow
        for(int j = 0; j < height; j ++){
            Pij[0] =0 -camera_height/2 + pixwidth*(j+0.5);//xcoordofcolumn
            Pij[2] = -1;
            
            float Ro[3] = {0,0,0};
            float *Rd;
            Rd = vector_scale(Pij,1/(vector_length(Pij)));
            
            float *color;
            color = raycast(Ro,Rd);
            
            temp[ptr++] = (int)(color[0] * 255);
            temp[ptr++] = (int)(color[1] * 255);
            temp[ptr++] = (int)(color[2] * 255);

            //printf("color[%d][%d]:{%f,%f,%f}; ",i,j,color[0], color[1], color[2]);

            //printf("color[%d][%d]:{%d,%d,%d}; ",i,j,(int)(color[0] * 255),(int)(color[1] * 255),(int)(color[2] * 255));
            
            //printf("color[%d][%d]:{%c,%c,%c}; ",i,j,(unsigned char)(int)(color[0] * 255),(unsigned char)(int)(color[1] * 255),(unsigned char)(int)(color[2] * 255));

            
            
        }
        //printf("\n");
    }
    
    for(int i = 0; i < width*height*3; i++){
        img[i] = (unsigned char)temp[i];
    }
    
    
    //output p3.ppm
    FILE *outfile = fopen(output_file, "w");
    if (outfile==NULL) {
        printf("Error: FILE error\n");
        exit(0);
    }
    writeHeader(outfile, 3, width, height, 255);
    writeP3(outfile, img, width, height);
    fclose(outfile);
}

float plane_intersection(float Ro[3],float Rd[3],Object obj){
    //printf("-(%f*(%f-%f) + %f*(%f-%f) + %f*(%f-%f)) / (%f*%f + %f*%f + %f*%f)",obj.normal[0],Ro[0],obj.position[0],obj.normal[1],Ro[1],obj.position[1],obj.normal[2],Ro[2],obj.position[2],obj.normal[0],Rd[0],obj.normal[1],Rd[1],obj.normal[2],Rd[2]);
    return -(obj.normal[0]*(Ro[0]-obj.position[0]) + obj.normal[1]*(Ro[1]-obj.position[1]) + obj.normal[2]*(Ro[2]-obj.position[2])) /
        (obj.normal[0]*Rd[0] + obj.normal[1]*Rd[1] + obj.normal[2]*Rd[2]);
}


float sphere_intersection(float Ro[3],float Rd[3],Object obj){
    float t_close = dot_product(Rd,vector_substract(obj.position,Ro));
    
    float* x_close = vector_add(Ro,vector_scale(Rd,t_close));
    
    float distance = point_distance(x_close,obj.position);
    
    //printf("the distance is %f",distance);
    
    if(distance > obj.radius){
        return 99999.9;
    } else if (distance == obj.radius){
        return t_close;
    } else if (distance < obj.radius){
        int a = sqrt(obj.radius*obj.radius - distance*distance);
        return t_close - a;
    }
    return 99999.9;
}


float dot_product(float x[3],float y[3]){
    return x[0] * y[0] + x[1] * y[1] + x[2] * y[2];
}

float vector_length(float x[3]){
    return sqrt(x[0] * x[0] + x[1] * x[1] + x[2] * x[2]);
}

float* vector_add(float x[3], float y[3]){
    static float result[3];
    result[0] = x[0] + y[0];
    result[1] = x[1] + y[1];
    result[2] = x[2] + y[2];
    return result;
}

float* vector_substract(float x[3], float y[3]){
    static float result[3];
    result[0] = x[0] - y[0];
    result[1] = x[1] - y[1];
    result[2] = x[2] - y[2];
    return result;
}

float* vector_scale(float x[3], float a){
    static float result[3];
    result[0] = x[0] * a;
    result[1] = x[1] * a;
    result[2] = x[2] * a;
    return result;
}

float point_distance(float x[3], float y[3]){
    return sqrt((x[0]-y[0])*(x[0]-y[0]) + (x[1]-y[1])*(x[1]-y[1]) + (x[1]-y[1])*(x[1]-y[1]));
}

int writeHeader(FILE *f, int magicNum, int w, int h, int color) {
    if (f==NULL) {
        printf("FILE error\n");
        exit(0);
    }
    
    if (magicNum==3) {
        fprintf(f, "P3\n");
    } else if (magicNum==6) {
        fprintf(f, "P6\n");
    } else {
        printf("magicNum can only be P3 or P6\n");
        exit(0);
    }
    
    fprintf(f, "%d %d\n", w, h);
    fprintf(f, "%d\n", color);
    return 0;
}

int writeP3(FILE* f, unsigned char* img, int w, int h) {
    int i,j;
    for(i=0;i<h;i++) {
        for(j=0;j<w;j++) {
            fprintf(f, "%d ",img[i*w*3+3*j]);
            fprintf(f, "%d ",img[i*w*3+3*j+1]);
            fprintf(f, "%d ",img[i*w*3+3*j+2]);
        }
        fprintf(f, "\n");
    }
    return 0;
}
