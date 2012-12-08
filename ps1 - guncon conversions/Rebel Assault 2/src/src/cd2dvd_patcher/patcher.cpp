#include <stdio.h>
#include <memory.h>

FILE *fp;
char file_name[4096];

unsigned char image[0x100000 * 16];
int image_size;

unsigned char patch_src[512][4096];
unsigned char patch_src_wild[512][4096];
unsigned char patch_dst[512][4096];
unsigned char patch_dst_wild[512][4096];
int patch_fixed[512];

int patch_src_len[512];
int patch_dst_len[512];

int patch_index[512];
int patch_count;

int lcv;
int ram_offset;

int main( int argc, char** argv )
{
	memset( patch_fixed, -1, sizeof(patch_fixed) );


	sprintf( file_name, "patch//%s", argv[1] );
	fp = fopen( file_name, "r" );
	if( !fp ) return -1;

	while( !feof(fp) )
	{
		fgets( file_name, 4096, fp );
		lcv=0;

		if( file_name[0] == 0x0d || file_name[0] == 0x0a )
			continue;


		if( file_name[0] == ';' )
			continue;


		// get code
		while( file_name[lcv] != 0x0a && file_name[lcv] != 0x0d )
		{
			int value;

			value = 0;


			if( file_name[0] == '#' )
			{
				sscanf( file_name+1, "%X", &ram_offset );
				break;
			}


			if( file_name[0] == '$' )
			{
				sscanf( file_name+1, "%X", &value );
				value -= ram_offset;

				patch_fixed[patch_count] = value;

				break;
			}


			if( file_name[lcv+0] == '*' &&
					file_name[lcv+1] == '*' )
			{
				patch_src_wild[patch_count][lcv/2] = 1;
			}
			else
			{
				sscanf( file_name+lcv, "%02X", &value );

				patch_src[patch_count][lcv/2] = value;
			}

			patch_src_len[patch_count]++;
			lcv += 2;
		}


		fgets( file_name, 4096, fp );
		lcv=0;

		while( file_name[lcv] != 0x0a && file_name[lcv] != 0x0d )
		{
			int value;

			value = 0;
			if( file_name[lcv+0] == '*' &&
					file_name[lcv+1] == '*' )
			{
				patch_dst_wild[patch_count][lcv/2] = 1;
			}
			else
			{
				sscanf( file_name+lcv, "%02X", &value );

				patch_dst[patch_count][lcv/2] = value;
			}
			
			patch_dst_len[patch_count]++;
			lcv += 2;
		}

		patch_count++;
	}

	fclose( fp );



	sprintf( file_name, "%s", argv[2] );
	fp = fopen( file_name, "rb" );
	if( !fp ) return -1;

	if( argc==4 )
	{
		int seeker;

		sscanf( argv[3], "%X", &seeker );
		fseek( fp, seeker, SEEK_SET );
	}
	image_size = fread( image, 1, 0x100000 * 16, fp );

	fclose( fp );

	

	int patches;
	
	patches = patch_count;
	for( lcv=0; lcv<patch_count; lcv++ )
	{
		if( patch_fixed[lcv] != -1 )
		{
			int pos;

			pos = patch_fixed[lcv];
			for( int lcv2=0; lcv2<patch_dst_len[lcv]; lcv2++ )
			{
				if( patch_dst_wild[lcv][lcv2] == 0 )
					image[pos++] = patch_dst[lcv][lcv2];
				else
					pos++;
			}

			patches--;
		}
	}


	lcv = 0;
	while( patches && lcv<image_size )
	{
		int start_lcv;

		start_lcv = lcv;
		for( int lcv2=0; lcv2<patch_count; lcv2++ )
		{
			//if( lcv >= 0x22853 )
				//lcv2 += 0;

			int idx;

			idx = 0;
			lcv = start_lcv;

			for( int lcv3=0; lcv3<patch_src_len[lcv2]; lcv3++ )
			{
				if( patch_src[lcv2][idx] == image[lcv] ||
						patch_src_wild[lcv2][idx] == 1 )
				{
					idx++;
					lcv++;

					if( patch_src_len[lcv2] == idx )
					{
						patch_src_len[lcv2] = 0;

						lcv = start_lcv;
						for( int lcv3=0; lcv3<patch_dst_len[lcv2]; lcv3++ )
						{
							if( patch_dst_wild[lcv2][lcv3] == 0 )
								image[lcv++] = patch_dst[lcv2][lcv3];
							else
								lcv++;
						}

						start_lcv = lcv-1;
						patches--;
						break;
					}
				}

				// mismatch
				else
					break;
			}
		}

		lcv = start_lcv + 1;
	} // check all patches


	sprintf( file_name, "%s", argv[2] );
	fp = fopen( file_name, "rb+" );
	if( !fp ) return -1;

	if( argc==4 )
	{
		int seeker;

		sscanf( argv[3], "%X", &seeker );
		fseek( fp, seeker, SEEK_SET );
	}
	fwrite( image, 1, image_size, fp );

	fclose( fp );
	return 0;
}