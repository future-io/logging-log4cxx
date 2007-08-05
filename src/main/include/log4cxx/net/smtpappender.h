/*
 * Licensed to the Apache Software Foundation (ASF) under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.
 * The ASF licenses this file to You under the Apache License, Version 2.0
 * (the "License"); you may not use this file except in compliance with
 * the License.  You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef _LOG4CXX_NET_SMTP_H
#define _LOG4CXX_NET_SMTP_H


#include <log4cxx/appenderskeleton.h>
#include <log4cxx/helpers/cyclicbuffer.h>
#include <log4cxx/spi/triggeringeventevaluator.h>


namespace log4cxx
{
        namespace net
        {
                class SMTPAppender;
                typedef helpers::ObjectPtrT<SMTPAppender> SMTPAppenderPtr;

                /**
                Send an e-mail when a specific logging event occurs, typically on
                errors or fatal errors.
                <p>The number of logging events delivered in this e-mail depend on
                the value of <b>BufferSize</b> option. The
                <code>SMTPAppender</code> keeps only the last
                <code>BufferSize</code> logging events in its cyclic buffer. This
                keeps memory requirements at a reasonable level while still
                delivering useful application context.
                */
                class LOG4CXX_EXPORT SMTPAppender : public AppenderSkeleton
                {
                private:
                        LogString to;
                        LogString from;
                        LogString subject;
                        LogString smtpHost;
                        int bufferSize; // 512
                        bool locationInfo;
                        helpers::CyclicBuffer cb;
                        void * session;
                        LogString encoding;
                        LogString charset;

                protected:
                        spi::TriggeringEventEvaluatorPtr evaluator;

                public:
                        DECLARE_LOG4CXX_OBJECT(SMTPAppender)
                        BEGIN_LOG4CXX_CAST_MAP()
                                LOG4CXX_CAST_ENTRY(SMTPAppender)
                                LOG4CXX_CAST_ENTRY_CHAIN(AppenderSkeleton)
                        END_LOG4CXX_CAST_MAP()

                        SMTPAppender();
                        /**
                        The default constructor will instantiate the appender with a
                        spi::TriggeringEventEvaluator that will trigger on events with
                        level ERROR or higher.*/
                        SMTPAppender(log4cxx::helpers::Pool& p);

                        /**
                        Use <code>evaluator</code> passed as parameter as the
                        spi::TriggeringEventEvaluator for this net::SMTPAppender.
                        */
                        SMTPAppender(spi::TriggeringEventEvaluatorPtr evaluator);

                        ~SMTPAppender();

                        /**
                         Set options
                        */
                        virtual void setOption(const LogString& option, const LogString& value);

                        /**
                        Activate the specified options, such as the smtp host, the
                        recipient, from, etc.
                        */
                        virtual void activateOptions(log4cxx::helpers::Pool& p);

                        /**
                        Perform SMTPAppender specific appending actions, mainly adding
                        the event to a cyclic buffer and checking if the event triggers
                        an e-mail to be sent. */
                        virtual void append(const spi::LoggingEventPtr& event, log4cxx::helpers::Pool& p);

                        /**
                        This method determines if there is a sense in attempting to append.
                        <p>It checks whether there is a set output target and also if
                        there is a set layout. If these checks fail, then the boolean
                        value <code>false</code> is returned. */
                        bool checkEntryConditions();

                        virtual void close();

                        std::vector<LogString> parseAddress(const LogString& addressStr);

                        /**
                        Returns value of the <b>To</b> option.
                        */
                        inline const LogString& getTo() const
                                { return to; }

                        /**
                        The <code>SMTPAppender</code> requires a {@link
                        Layout layout}.  */
                        virtual bool requiresLayout() const
                                { return true; }

                        /**
                        Send the contents of the cyclic buffer as an e-mail message.
                        */
                        void sendBuffer();

                        /**
                        Returns value of the <b>Charset</b> option.
                        */
                        inline const LogString& getCharset() const
                                { return charset; }

                        /**
                        Returns value of the <b>Encoding</b> option.
                        */
                        inline const LogString& getEncoding() const
                                { return encoding; }

                        /**
                        Returns value of the <b>EvaluatorClass</b> option.
                        */
                        LogString getEvaluatorClass();

                        /**
                        Returns value of the <b>From</b> option.
                        */
                        inline const LogString& getFrom() const
                                { return from; }

                        /**
                        Returns value of the <b>Subject</b> option.
                        */
                        inline const LogString& getSubject() const
                                { return subject; }

                        /**
                        The <b>Charset</b> option takes a string value which should be the
                        charset of the mail (<code>us-ascii</code>, <code>iso8859_1</code>,
                        <code>iso8859_2</code>, <code>iso8859_3</code>).
                        */
                        inline void setCharset(const LogString& charset)
                                { this->charset.assign(charset); }

                        /**
                        The <b>Encoding</b> option takes a string value which should be the
                        encoding type of the mail (<code>7bit</code>, <code>8bit</code>,
                        <code>base64</code>, <code>binary</code>, <code>quoted</code>).
                        */
                        inline void setEncoding(const LogString& charset)
                                { this->encoding.assign(encoding); }

                        /**
                        The <b>From</b> option takes a string value which should be a
                        e-mail address of the sender.
                        */
                        inline void setFrom(const LogString& from)
                                { this->from.assign(from); }

                        /**
                        The <b>Subject</b> option takes a string value which should be a
                        the subject of the e-mail message.
                        */
                        inline void setSubject(const LogString& subject)
                                { this->subject.assign(subject); }

                        /**
                        The <b>BufferSize</b> option takes a positive integer
                        representing the maximum number of logging events to collect in a
                        cyclic buffer. When the <code>BufferSize</code> is reached,
                        oldest events are deleted as new events are added to the
                        buffer. By default the size of the cyclic buffer is 512 events.
                        */
                        void setBufferSize(int bufferSize);

                        /**
                        The <b>SMTPHost</b> option takes a string value which should be a
                        the host name of the SMTP server that will send the e-mail message.
                        */
                        inline void setSMTPHost(const LogString& smtpHost)
                                { this->smtpHost.assign(smtpHost); }

                        /**
                        Returns value of the <b>SMTPHost</b> option.
                        */
                        inline const LogString& getSMTPHost() const
                                { return smtpHost; }

                        /**
                        The <b>To</b> option takes a string value which should be a
                        comma separated list of e-mail address of the recipients.
                        */
                        inline void setTo(const LogString& to)
                                { this->to.assign(to); }

                        /**
                        Returns value of the <b>BufferSize</b> option.
                        */
                        inline int getBufferSize() const
                                { return bufferSize; }

                        /**
                        The <b>EvaluatorClass</b> option takes a string value
                        representing the name of the class implementing the
                        spi::TriggeringEventEvaluator interface. A corresponding object will
                        be instantiated and assigned as the triggering event evaluator
                        for the SMTPAppender.
                        */
                        void setEvaluatorClass(const LogString& value);

                        /**
                        The <b>LocationInfo</b> option takes a boolean value. By
                        default, it is set to false which means there will be no effort
                        to extract the location information related to the event. As a
                        result, the layout that formats the events as they are sent out
                        in an e-mail is likely to place the wrong location information
                        (if present in the format).

                        <p>Location information extraction is comparatively very slow and
                        should be avoided unless performance is not a concern.
                        */
                        inline void setLocationInfo(bool locationInfo)
                                { this->locationInfo = locationInfo; }

                        /**
                        Returns value of the <b>LocationInfo</b> option.
                        */
                        inline bool getLocationInfo() const
                                { return locationInfo; }

                private:
                        SMTPAppender(const SMTPAppender&);
                        SMTPAppender& operator=(const SMTPAppender&);

                }; // class SMTPAppender

                class LOG4CXX_EXPORT DefaultEvaluator :
                        public virtual spi::TriggeringEventEvaluator,
                        public virtual helpers::ObjectImpl
                {
                public:
                        DECLARE_LOG4CXX_OBJECT(DefaultEvaluator)
                        BEGIN_LOG4CXX_CAST_MAP()
                                LOG4CXX_CAST_ENTRY(spi::TriggeringEventEvaluator)
                        END_LOG4CXX_CAST_MAP()

                        DefaultEvaluator();
                        DefaultEvaluator(log4cxx::helpers::Pool& p);

                        /**
                        Is this <code>event</code> the e-mail triggering event?
                        <p>This method returns <code>true</code>, if the event level
                        has ERROR level or higher. Otherwise it returns
                        <code>false</code>.
                        */
                        virtual bool isTriggeringEvent(const spi::LoggingEventPtr& event);
                }; // class DefaultEvaluator
        }  // namespace net
} // namespace log4cxx

#endif // _LOG4CXX_NET_SMTP_H