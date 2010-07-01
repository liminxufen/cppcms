//
//  Copyright (c) 2010 Artyom Beilis (Tonkikh)
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOSTER_CALLBACK_H
#define BOOSTER_CALLBACK_H

#include <stdexcept>
#include <booster/intrusive_ptr.h>
#include <booster/refcounted.h>

namespace booster {
	template<typename Type>
	class callback;

	class bad_callback_call : public std::runtime_error {
	public:
		bad_callback_call() : 
			std::runtime_error("bad_callback_call")
		{
		}
	};


	#ifdef BOOSTER_DOXYGEN_DOCS
	///
	/// \brief This is Booster's implementation of std::tr1::callback/booster::callback.
	///
	/// This callback is created from generic object that can be "called" i.e. 
	/// a class with operator() or callback pointer that has same signature as
	/// the callback.
	///
	/// See: http://www.boost.org/doc/html/callback.html
	///
	/// Notes:
	///
	/// - this code is not taken from Boost and has slightly different interface.
	/// - as most of compilers do not support Variadic templates yet, this class
	///   is explicitly specialized for Params of size 0 to 8. So maximal amout
	///   of parameters that can be used is 8.
	///
	///
	template<typename Result,typename ...Params>			
	class callback<Result(Params...)>				
	{									
	public:									
		///
		/// Type of result, for use with boost::bind
		///
		typedef Result result_type;		
		///
		/// Default constructor, creates an empty callbacks
		///			
		callback();
		///
		/// Creates a callback from a callbackal \a func of type F. func
		/// should be copyable. It is copied and stored inside callback object.
		///
		template<typename F>						
		callback(F func);
		
		///
		/// Copy callback, Copies underlying callbackal object.
		///
		callback(callback const &other);

		///
		/// Assign a callbackal \a func of type F. func
		/// should be copyable. It is copied and stored inside callback object.
		///
		template<typename F>						
		callback const &operator=(F func);

		///
		/// Assignment operator. Copies underlying callbackal object.
		/// 
		callback const &operator=(callback const &other);

		///
		/// Calls underling callbackal object. If the callback is empty, throws bad_callback_call.
		///
		result_type operator()(Params... params) const;
		///
		/// Return true if the callback is empty
		///
		bool empty() const;
		///
		/// Returns true if the callback is not empty
		///
		operator bool() const;

		///
		/// Swaps two callbackal object. Does not throw.
		///
		void swap(callback &other);
	};			

	#else

	#define BOOSTER_CALLBACK							\
	template<typename Result BOOSTER_TEMPLATE_PARAMS >			\
	class callback<Result(BOOSTER_TYPE_PARAMS)>				\
	{									\
	public:									\
		typedef Result result_type;					\
		struct callable : public refcounted {				\
			virtual Result call(BOOSTER_TYPE_PARAMS) =0;		\
			virtual ~callable(){}					\
		};								\
										\
		template<typename R,typename F>					\
		struct callable_impl : public callable {			\
			F func;							\
			callable_impl(F f) : func(f){}				\
			virtual R call(BOOSTER_TYPE_PARAMS) 			\
			{  return func(BOOSTER_CALL_PARAMS); }			\
		};								\
		template<typename F>						\
		struct callable_impl<void,F> : public callable {		\
			F func;							\
			callable_impl(F f) : func(f){}				\
			virtual void call(BOOSTER_TYPE_PARAMS) 			\
			{  func(BOOSTER_CALL_PARAMS); }				\
		};								\
		callback(){}							\
		template<typename F>						\
		callback(F func) : call_ptr(new callable_impl<Result,F>(func)) 	\
		{}								\
		callback(callback const &other) : call_ptr(other.call_ptr) {}	\
		template<typename F>						\
		callback const &operator=(F func)				\
		{ 								\
			call_ptr = new callable_impl<Result,F>(func);		\
			return *this;						\
		}								\
		callback const &operator=(callback const &other)		\
		{ 								\
			if(this != &other) { call_ptr=other.call_ptr; } 	\
			return *this;						\
		}								\
		Result operator()(BOOSTER_TYPE_PARAMS) const			\
		{ 								\
			if(!call_ptr.get()) throw bad_callback_call();		\
			return call_ptr->call(BOOSTER_CALL_PARAMS); 		\
		}								\
		bool empty() const { return call_ptr.get()==0; }		\
		operator bool() const { return !empty(); }			\
		void swap(callback &other) { call_ptr.swap(other.call_ptr); }	\
	private:								\
		intrusive_ptr<callable> call_ptr;					\
	};									\

	#define BOOSTER_TEMPLATE_PARAMS
	#define BOOSTER_TYPE_PARAMS
	#define BOOSTER_CALL_PARAMS
	BOOSTER_CALLBACK
	#undef BOOSTER_TEMPLATE_PARAMS
	#undef BOOSTER_TYPE_PARAMS
	#undef BOOSTER_CALL_PARAMS

	#define BOOSTER_TEMPLATE_PARAMS ,typename P1
	#define BOOSTER_TYPE_PARAMS  P1 a1
	#define BOOSTER_CALL_PARAMS a1
	BOOSTER_CALLBACK
	#undef BOOSTER_TEMPLATE_PARAMS
	#undef BOOSTER_TYPE_PARAMS
	#undef BOOSTER_CALL_PARAMS

	#define BOOSTER_TEMPLATE_PARAMS ,typename P1,typename P2
	#define BOOSTER_TYPE_PARAMS  P1 a1,P2 a2
	#define BOOSTER_CALL_PARAMS a1,a2
	BOOSTER_CALLBACK
	#undef BOOSTER_TEMPLATE_PARAMS
	#undef BOOSTER_TYPE_PARAMS
	#undef BOOSTER_CALL_PARAMS
			
	#define BOOSTER_TEMPLATE_PARAMS ,typename P1,typename P2,typename P3
	#define BOOSTER_TYPE_PARAMS  P1 a1,P2 a2,P3 a3
	#define BOOSTER_CALL_PARAMS a1,a2,a3
	BOOSTER_CALLBACK
	#undef BOOSTER_TEMPLATE_PARAMS
	#undef BOOSTER_TYPE_PARAMS
	#undef BOOSTER_CALL_PARAMS
	
	#define BOOSTER_TEMPLATE_PARAMS ,typename P1,typename P2,typename P3,typename P4
	#define BOOSTER_TYPE_PARAMS  P1 a1,P2 a2,P3 a3,P4 a4
	#define BOOSTER_CALL_PARAMS a1,a2,a3,a4
	BOOSTER_CALLBACK
	#undef BOOSTER_TEMPLATE_PARAMS
	#undef BOOSTER_TYPE_PARAMS
	#undef BOOSTER_CALL_PARAMS

	#define BOOSTER_TEMPLATE_PARAMS ,typename P1,typename P2,typename P3,typename P4,typename P5
	#define BOOSTER_TYPE_PARAMS  P1 a1,P2 a2,P3 a3,P4 a4,P5 a5
	#define BOOSTER_CALL_PARAMS a1,a2,a3,a4,a5
	BOOSTER_CALLBACK
	#undef BOOSTER_TEMPLATE_PARAMS
	#undef BOOSTER_TYPE_PARAMS
	#undef BOOSTER_CALL_PARAMS

	#define BOOSTER_TEMPLATE_PARAMS ,typename P1,typename P2,typename P3,typename P4,typename P5,typename P6
	#define BOOSTER_TYPE_PARAMS  P1 a1,P2 a2,P3 a3,P4 a4,P5 a5,P6 a6
	#define BOOSTER_CALL_PARAMS a1,a2,a3,a4,a5,a6
	BOOSTER_CALLBACK
	#undef BOOSTER_TEMPLATE_PARAMS
	#undef BOOSTER_TYPE_PARAMS
	#undef BOOSTER_CALL_PARAMS
	#define BOOSTER_TEMPLATE_PARAMS ,typename P1,typename P2,typename P3,typename P4,typename P5,typename P6,typename P7
	#define BOOSTER_TYPE_PARAMS  P1 a1,P2 a2,P3 a3,P4 a4,P5 a5,P6 a6,P7 a7
	#define BOOSTER_CALL_PARAMS a1,a2,a3,a4,a5,a6,a7
	BOOSTER_CALLBACK
	#undef BOOSTER_TEMPLATE_PARAMS
	#undef BOOSTER_TYPE_PARAMS
	#undef BOOSTER_CALL_PARAMS
	#define BOOSTER_TEMPLATE_PARAMS ,typename P1,typename P2,typename P3,typename P4,typename P5,typename P6,typename P7,typename P8
	#define BOOSTER_TYPE_PARAMS  P1 a1,P2 a2,P3 a3,P4 a4,P5 a5,P6 a6,P7 a7,P8 a8
	#define BOOSTER_CALL_PARAMS a1,a2,a3,a4,a5,a6,a7,a8
	BOOSTER_CALLBACK
	#undef BOOSTER_TEMPLATE_PARAMS
	#undef BOOSTER_TYPE_PARAMS
	#undef BOOSTER_CALL_PARAMS

	#undef BOOSTER_CALLBACK

	#endif // DOC

} // booster


#endif
