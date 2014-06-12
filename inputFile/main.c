int x ; // aaaa
int y ;
int main ( ) {
	int a [ 3 ] ;
	a [ 2 ] = a [ 0 ] + a [ 1 ] ;
	x = 1 ;
	y = 20 ;
	
	x = y + 1 ;
	
	if ( x > 0 ) {
	x = 100 ;
	} 
	else {
	x = 5 ; 
	}
	
	y = 1 + 5 ;
	
	while ( x > 5 )
	{
	y = x + 40 ;
	x = 3 ;
	}
	
	x = 1 * y ;
	
	return x + 3 ;
}

int doub ( int c , int d ) {
	
	return c * d ;
}
