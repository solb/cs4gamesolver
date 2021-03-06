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

#ifndef KAYLESSTATE_H
#define KAYLESSTATE_H

#include <string>
#include <utility>
#include <vector>

/**
Represents the Kayles game state at some fixed point in time.

@author Sol Boucher <slb1566@rit.edu>
@author Kyle Savarese <kms7341@rit.edu>
*/
class KaylesState
{
	public: //gameplay constants
		/** The maximum number of pins a player may topple per turn.
			*/
		static const int MAX_TAKEN=2;
		
		/** The minimum number of pins permissible to topple. */
		static const int MIN_TAKEN=1;
	
	public: //descriptions
		/** The possible "scores" resulting from a complete match */
		enum Score
		{
			LOSS=-1,
			TIE=0, //also known as "incomplete"
			VICTORY=1
		};
	
	private: //state
		/** Stores one pin count per group of pins */
		std::vector< int > pins;
		
		/** Whether the computer player is up */
		bool ourTurn;
		
		/** Caches the current hash code */
		int hashCode;
	
	public: //behavior
		/**
		Creates a new game given its initial circumstances.
		@param startingPins the initial pin arrangement
		@param weAreUp whether or not the "good guy" is up
		*/
		inline explicit KaylesState( const std::vector< int >& startingPins=
			std::vector< int >(), bool weAreUp=true );
		
		/**
		Creates the move resulting from taking away a specified number
			of pins.  This makes the most sense when a positive nu
			mber of them are taken, and preferably a legal number;
			however, this is not required.
		@pre the <tt>position</tt> is in range
		@post The new state reflects the fact that it is now the oppos
			ite player's turn.
		@param baseState the state on which to base this new one
		@param position the group of pins affected
		@param taken the number of from that group that were knocked d
			own
		*/
		KaylesState( const KaylesState& baseState, unsigned int
			position, int taken, int target );
		
		/**
		Destroys the game state.
		*/
		inline ~KaylesState( void );
		
		/**
		Judges whether the game is over.
		@return whether there are no pins left
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
		Counts the pin groups.
		@return how many groups exist
		*/
		inline int groupsOfPins( void ) const;
		
		/**
		Counts the pins in a group.
		@param group the group to examine
		@return how many pins occupy it, or <tt>-1</tt> if it doesn't
			exist
		*/
		inline int pinsInGroup( unsigned int group ) const;
		
		/**
		Returns all possible successor states.
		@return whatever might happen next
		*/
		void successors( std::vector< KaylesState >& result ) const;
		
		/**
		Produces a synopsis of this <tt>State</tt>'s particulars.
		@return the <tt>string</tt> representation
		*/
		std::string str( void ) const;
		
		/**
		Hashes the <tt>State</tt>.
		@pre <tt>hashCode</tt> is up to date
		@post The result is nonnegative.
		@return a hash code 
		*/
		inline int hash( void ) const;
		
		/**
		Checks identity
		@pre <tt>sorted</tt> is up to date
		@param another comparable <tt>State</tt>
		@return whether the turns and pin groups are the same
		*/
		inline bool operator==( const KaylesState& another ) const;
		
		/**
		Determines whether two game states are subsequent.
		@param first the original state
		@param next the new state
		@return whether they would appear in a game in sequence
		*/
		static bool areSubsequent( const KaylesState& first, const
			KaylesState& next );
		
		/**
		Finds the move made to get between two game states.
		@pre The states must be exactly one move apart.
		@param first the original state
		@param next the new state
		@return a <tt>std::vector</tt> containing the group from which one
			or more pins were removed, the first pin that was
			removed, and how many were taken, or an empty
			<tt>std::vector</tt> in the case of a poorly-phrased
			question
		*/
		static std::vector< int > diff( const KaylesState& first,
			const KaylesState& next );
	
	private: //helpers
		/**
		Recomputes hash code; must be called every time <tt>pins</tt>
			is mutated.
		*/
		void cacheHash( void );
};

/** @brief Constructor */
KaylesState::KaylesState( const std::vector< int >& startingPins, bool weAreUp ):
	pins( startingPins ), ourTurn( weAreUp ), hashCode( 0 )
{
	cacheHash();
}

/** @brief Destructor */
KaylesState::~KaylesState() {}

/** @brief Who won? */
KaylesState::Score KaylesState::scoreGame() const
{
	if( gameOver() )
		if( ourTurn ) return LOSS;
		else /*!ourTurn*/ return VICTORY;
	else /*!gameOver()*/ return TIE;
}

/** @brief Is it our turn? */
bool KaylesState::computersTurn() const
{
	return ourTurn;
}

/** @brief How many groups? */
int KaylesState::groupsOfPins() const
{
	return pins.size();
}

/** @brief How many pins? */
int KaylesState::pinsInGroup( unsigned int group ) const
{
	if( group>=pins.size() ) return -1;
	else return pins[group];
}

/** @brief Hashing */
int KaylesState::hash() const
{
	return hashCode;
}

/** @brief Same state? */
bool KaylesState::operator==( const KaylesState& another ) const
{
	return this->ourTurn==another.ourTurn &&
		this->pins.size()==another.pins.size() &&
		equal( this->pins.begin(), this->pins.end(),
		another.pins.begin() );
}

#endif
