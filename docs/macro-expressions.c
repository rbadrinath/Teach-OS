// Just use 'gcc -E <thisfilename>' to see the output.
//
// Beware, this file will not compile as a C program.
//         It is just to see how macros expand
//         
// In the comments  the arrows '->' are used to indicate immediate macro rewrite
// 
// For a documentation see 
//       https://gcc.gnu.org/onlinedocs/cpp/Macros.html#Macros
//
//
#define X 100           // X -> 100
#define BIG 1  	        // BIG -> 1
1. BIG

#define MYBIG BIG       // MYBIG -> BIG -> 1
2. MYBIG
		    
#define L_BIG 2	        // Where L_BIG appears replace it by 2
//
	// The bow are macro functions
	// NOTE: All arguments to a macro are completely macro-expanded 
	//       before they are substituted into the macro body.
	//       ... except...
#define NAMEIT(X)   X	// NAMEIT(BIG) -> NAMEIT(1) -> 1
3. NAMEIT(BIG)

#define DAY(X) "Today is " X // When expanding
				//DAY(X)
				//-> DAY(100)    :: Arguments expand first
				//-> "Today is " 100
4. DAY(X)

// # is used to say just quote the following literatal: MODIFIED EXPANSION RULE
#define DDAY(X) "Today is " #X //
				// DDAY(X)
				// Note :: Arg NOT expanded first if stringized!
				// -> "Today is " #X 
				// -> "Today is " "X"
5. DDAY(X)

// Therefore to expand and quote, take two steps:
#define EDDAY(X) DDAY(X)  // EDDAY(X)
			  // -> EDDAY(100) ->DDAY(100) -> #100 ->"100"
6. EDDAY(X)

// What happens to string around a string ??
#define STRINGIT(X) #X	// STRINGIT(BIG) -> #BIG ->"BIG"
7. STRINGIT(BIG)

#define EXPSTRINGIT(X) STRINGIT(X) 
				// EXPSTRINGIT (BIG)    : Arg substituted
			   	// -> EXPSTRINGIT ( 1 ) : because here is no "#"
			   	// -> STRINGIT(1) -> #1 -> "1"
8. EXPSTRINGIT(BIG)

#define QEXPSTRINGIT(X) EXPSTRINGIT(STRINGIT(X))
				// QEXPSTRINGIT(BIG)
				// -> QEXPSTRINGIT(1)
				// -> EXPSTRINGIT(STRINGIT(1))
				// -> EXPSTRINGIT( "1" )
				// -> STRINGIT("1")
				// -> #"1"
				// -> "\"1\""   : Notice nice escaping 
				// 		  of inner quote marks
9. QEXPSTRINGIT(BIG)
	
// ## is used for concatenation again DONT expand argument first.
#define CON(X)    L_##X // CON(BIG) 
			// ->L_BIG
			// ->2
10. CON(BIG)
