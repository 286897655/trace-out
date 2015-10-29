#pragma once

#include "defines.h"
#include "resource.h"


namespace pretty_output
{

	namespace impl
	{

		typedef struct _tlskey *tlskey_t;

		tlskey_t tls_new_key();
		void tls_delete_key(tlskey_t key);
		void *tls_get(tlskey_t key);
		void tls_set(tlskey_t key, void *data);


		template <typename Type_t>
		class tls
		{
		public:
			tls();
			~tls();

			void set(const Type_t &value);
			const Type_t &get() const;

		private:
			tls(const tls &another);
			tls &operator =(const tls &another);

#if defined(PRETTY_OUTPUT_CPP11)

			tls(tls &&another);
			tls &operator =(tls &&another);

#endif // defined(PRETTY_OUTPUT_CPP11)

			resource<tlskey_t> _key;
		};

	}

}


namespace pretty_output
{

	namespace impl
	{

		template <typename Type_t>
		tls<Type_t>::tls()
			: _key(tls_new_key(), tls_delete_key)
		{
		}


		template <typename Type_t>
		tls<Type_t>::~tls()
		{
			Type_t *value = static_cast<Type_t *>(tls_get(_key.get()));
			delete value;
		}


		template <typename Type_t>
		void tls<Type_t>::set(const Type_t &value)
		{
			Type_t *new_value = new Type_t(value);

			Type_t *old_value = static_cast<Type_t *>(tls_get(_key.get()));
			delete old_value;

			tls_set(_key.get(), new_value);
		}


		template <typename Type_t>
		const Type_t &tls<Type_t>::get() const
		{
			Type_t *value = static_cast<Type_t *>(tls_get(_key.get()));
			if (value == NULL)
			{
				value = new Type_t;
				tls_set(_key.get(), value);
			}

			return *value;
		}

	}

}

