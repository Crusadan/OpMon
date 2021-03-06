/*!
 * \file ATranslatable.hpp
 * \author BAKfr
 * \copyright GNU GPL v3.0
 * \date 12/01/18
*/
#pragma once

namespace OpMon {
    /*!
     * \namespace OpMon::I18n
     * \brief Contains internationalisation tools.
     */
    namespace I18n {

        /*!
         * \class ATranslatable ATranslatable.hpp "opmon/start/i18n/ATranslatable.hpp"
         * \brief Base class for any element requiring to reload when the lang changes.
         */
        class ATranslatable {
          protected:
            ATranslatable();
            virtual ~ATranslatable();

          public:
            /*!
             * \brief Method called after the language has been changed.
             */
            virtual void onLangChanged() = 0;
        };

    } // namespace I18n
} // namespace OpMon
