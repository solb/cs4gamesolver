/*
 * Copyright (C) 2012 Sol Boucher and Kyle Savarese
 * This is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with it.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef CONNECT3STATE_H
#define CONNECT3STATE_H

#include <cassert>
#include <string>
#include <vector>

/**
Represents the Connect-3 game state at some fixed point in time.

@author Sol Boucher <slb1566@rit.edu>
*/
class Connect3State
{
	public: //gameplay constants
		/** How many must be connected to win. */
		static const int CONNECTABLE=3;
		
		/** The character that indicates an empty space */
		static const char PLACEHOLDER='.';
		
		/** The characters appearing in the game board */
		static const char SYMBOLS[2];
		
		/** The character printed for each empty space */
		static const char PRINTHOLDER=' ';
		
		/** The character separating columns in printout */
		static const char PRINTVBAR='|';
		
		/** The character running along the bottom of the printout */
		static const char PRINTFOOTER='-';
		
		/** The major size */
		const unsigned int COLUMNS;
		
		/** The minor size */
		const unsigned int ELEMENTS;
		
	public: //descriptions
		/** The possible "scores" resulting from a complete match */
		enum Score
		{
			LOSS=-1,
			TIE=0, //can also mean incomplete
			VICTORY=1
		};
	
	private: //state
		/** The index of my symbol in <tt>SYMBOLS</tt> */
		int mySymbol;
		
		/** Stores one pin count per group of pins */
		std::vector< std::vector< char > > board;
		
		/** Whether the computer player is up */
		bool ourTurn;
		
		/** The victor of this particular round. */
		Score finalOutcome;
		
		/** Caches the current hash code */
		int hashCode;
	
	public: //behavior
		/**
		Creates a new game given its initial circumstances.
		@pre <tt>original</tt> is column-major and doesn't exceed
			bounds
		@pre <tt>original</tt> contains only elements of SYMBOLS
		@param columnCount how many columns per board
		@param elementCount how many elements per column
		@param original the starting board state
		@param weAreUp whether or not the "good guy" is up
		*/
		Connect3State( unsigned int columnCount=0,
			unsigned int elementCount=0, 
			const std::vector< std::vector< char > >&
				original=std::vector< std::vector< char > >(),
			bool weAreUp=true );
		
		/**
		Creates the move resulting from marking the top of the specifi
			fied column of the board.
		@pre the <tt>column</tt> is in range and not yet full
		@post The new state reflects the fact that it is now the oppos
			ite player's turn.
		@param baseState the state on which to base this new one
		@param column where the new mark is to be placed
		*/
		Connect3State( const Connect3State& baseState,
			unsigned int column );
		/**
		Destroys the game state.
		*/
		inline ~Connect3State( void );
		
		/**
		Judges whether the game is over.
		@return whether someone has won or the board is full
		*/
		bool gameOver( void ) const;
		
		/**
		Devines the match score, which is only meaningful if the game 
			is over.
		@return the score: <tt>Score::VICTORY</tt> for our victory, <t
			t>Score::LOSS</tt> for opponent's victory, or <tt>Scor
			e::TIE</tt> for an unterminated game
		*/
		inline Score scoreGame( void ) const;
		
		/**
		Determines whether it is our turn.
		@return whether the "good guy" is taking his turn
		*/
		inline bool computersTurn( void ) const;
		
		/**
		Asks whether there's space in a particular column to accomodat
			e another piece.
		@pre <tt>column</tt> is in range.
		@param column dat column
		@return whether it'll fit
		*/
		inline bool hasSpaceAt( unsigned int column ) const;
		
		/**
		Returns all possible successor states.
		@return whatever might happen next
		*/
		void successors( std::vector< Connect3State >& result ) const;
		
		/**
		Produces a synopsis of this <tt>State</tt>'s particulars.
		@return the <tt>string</tt> representation
		*/
		std::string str( void ) const;
		
		/**
		Hashes the <tt>State</tt>.
		@pre <tt>hashCode</tt> is up to date
		@post The result is non-negative.
		@return a hash code 
		*/
		inline int hash( void ) const;
		
		/**
		Checks identity.
		@pre <tt>sorted</tt> is up to date
		@param another comparable <tt>State</tt>
		@return whether the turns and pin groups are the same
		*/
		bool operator==( const Connect3State& another ) const;
		
		/**
		Performs assignment.
		@pre The two instances' constant data match
		@param another a source <tt>State</tt>
		@return the destination object
		*/
		Connect3State& operator=( const Connect3State& another );
		
		/**
		Determines whether two game states are subsequent.
		@param first the original state
		@param next the new state
		@return whether they would appear in a game in sequence
		*/
		static bool areSubsequent( const Connect3State& first,
			const Connect3State& next );
		
		/**
		Finds the move made to get between two game states.
		@pre The states must be exactly one move apart.
		@param first the original state
		@param next the new state
		@return the column to which an addition was made
		*/
		static int diff( const Connect3State& first,
			const Connect3State& next );
	
		/**
		Determines whether the given character is a valid board
			marking.
		Note that <tt>PLACEHOLDER</tt> is *not* valid.
		@param character to check
		@return whether it should be present 
		*/
		inline static bool validChar( char character );
	
	private: //helpers
		/**
		Recomputes hash code; must be called every time <tt>pins</tt>
		is mutated.
		*/
		void cacheHash( void );
		
		/**
		Recomputes the game's winner; must be called after mutation.
		It only checks the entire board if <tt>baseCol</tt> isn't
		supplied.
		Otherwise, only combinations including <tt>baseCol</tt> are
		considered.
		@pre <tt>baseCol</tt>, if supplied, is positive and in range
		@pre <tt>baseEl</tt> should normally be omitted, but must be
			in range
		@param baseCol the column that was just modified
		@param baseEl the corresponding element of that column
		@return a score, with <tt>TIE</tt> representing a nonterminal
			state
		*/
		Score computeWinner( int baseCol=-1, int baseEl=-1 );
};

/** @brief Destructor */
Connect3State::~Connect3State() {}

/** @brief Who won? */
Connect3State::Score Connect3State::scoreGame() const
{
	return finalOutcome;
}

/** @brief Is it our turn? */
bool Connect3State::computersTurn() const
{
	return ourTurn;
}

/** @brief Will it fit? */
bool Connect3State::hasSpaceAt( unsigned int column ) const
{
	assert( column<COLUMNS );
	
	return board[column].size()<ELEMENTS; //room to grow
}

/** @brief Hashing */
int Connect3State::hash() const
{
	return hashCode;
}

/** @brief Board-building check */
bool Connect3State::validChar( char character )
{
	return character==SYMBOLS[0] || character==SYMBOLS[1];
}

#endif
