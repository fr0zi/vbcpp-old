#ifndef CREFCOUNTER_HPP_INCLUDED
#define CREFCOUNTER_HPP_INCLUDED

#include <stdio.h>

#define DEBUG_MODE

class CReferenceCounter
{
	public:

		CReferenceCounter() : _refCounter(1)
		{
		    #ifdef DEBUG_MODE
                fprintf(stdout, "Creating Reference Counted object\n");
            #endif
		}


		virtual ~CReferenceCounter()
		{
		    #ifdef DEBUG_MODE
                fprintf(stdout, "Destroying Reference Counted object\n");
            #endif
		}


		void grab() const
		{
			++_refCounter;
		}


		bool drop() const
		{
			--_refCounter;

			if( !_refCounter )
			{
				delete this;
				return true;
			}

			return false;
		}


		unsigned int getReferenceCount() const
		{
			return _refCounter;
		}


	private:

		mutable unsigned int _refCounter;
};

#endif // CREFCOUNTER_HPP_INCLUDED
