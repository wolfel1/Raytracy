//
// Basic instrumentation profiler by The Cherno edited by Wolfram Woerner

#pragma once

#include <string>
#include <chrono>
#include <algorithm>
#include <fstream>
#include <sys/stat.h>

#include <thread>



namespace raytracy {
	struct ProfileResult
	{
		std::string Name;
		long long Start, End;
		std::thread::id ThreadID;
	};

	struct InstrumentationSession
	{
		std::string Name;
	};

	class Instrumentor
	{
	private:
		std::mutex m_Mutex;
		InstrumentationSession* m_CurrentSession;
		std::ofstream m_OutputStream;
		int m_ProfileCount;
	public:
		Instrumentor()
			: m_CurrentSession(nullptr), m_ProfileCount(0) {
		}

		void BeginSession(const std::string& name, const std::string& filepath = "results.json") {
			std::lock_guard lock(m_Mutex);
			if (m_CurrentSession) {
				// If there is already a current session, then close it before beginning new one.
				// Subsequent profiling output meant for the original session will end up in the
				// newly opened session instead.  That's better than having badly formatted
				// profiling output.
				if (Log::GetBaseLogger()) { // Edge case: BeginSession() might be before Log::Init()
					RTY_BASE_ERROR("Instrumentor::BeginSession('{0}') when session '{1}' already open.", name, m_CurrentSession->Name);
				}
				InternalEndSession();
			}
			std::string dir = "profiling";
			m_OutputStream.open("profiling/" + filepath);
			if (!m_OutputStream.is_open()) {
				_mkdir(dir.c_str());
			}
			if (m_OutputStream.is_open()) {
				m_CurrentSession = new InstrumentationSession({ name });
				WriteHeader();
			} else {
				if (Log::GetBaseLogger()) { // Edge case: BeginSession() might be before Log::Init()
					RTY_BASE_ERROR("Instrumentor could not open results file '{0}'.", filepath);
				}
			}
		}

		void EndSession() {
			std::lock_guard lock(m_Mutex);
			InternalEndSession();
		}

		void WriteProfile(const ProfileResult& result) {
			if (m_ProfileCount++ > 0) {

				m_OutputStream << ",";
			}

			std::stringstream json;

			std::string name = result.Name;
			std::replace(name.begin(), name.end(), '"', '\'');

			json << "{";
			json << "\"cat\":\"function\",";
			json << "\"dur\":" << (result.End - result.Start) << ',';
			json << "\"name\":\"" << name << "\",";
			json << "\"ph\":\"X\",";
			json << "\"pid\":0,";
			json << "\"tid\":" << result.ThreadID << ",";
			json << "\"ts\":" << result.Start;
			json << "}";

			std::lock_guard lock(m_Mutex);
			if (m_CurrentSession) {
				m_OutputStream << json.str();
				m_OutputStream.flush();
			}
		}

		static Instrumentor& Get() {
			static Instrumentor instance;
			return instance;
		}

	private:

		void WriteHeader() {
			m_OutputStream << "{\"otherData\": {},\"traceEvents\":[";
			m_OutputStream.flush();
		}

		void WriteFooter() {
			m_OutputStream << "]}";
			m_OutputStream.flush();
		}
		// Note: you must already own lock on m_Mutex before
		// calling InternalEndSession()
		void InternalEndSession() {
			if (m_CurrentSession) {
				WriteFooter();
				m_OutputStream.close();
				delete m_CurrentSession;
				m_CurrentSession = nullptr;
				m_ProfileCount = 0;
			}
		}
	};

	class InstrumentationTimer
	{
	public:
		InstrumentationTimer(const char* name)
			: m_Name(name), m_Stopped(false) {
			m_StartTimepoint = std::chrono::high_resolution_clock::now();
		}

		~InstrumentationTimer() {
			if (!m_Stopped)
				Stop();
		}

		void Stop() {
			auto endTimepoint = std::chrono::high_resolution_clock::now();

			long long start = std::chrono::time_point_cast<std::chrono::microseconds>(m_StartTimepoint).time_since_epoch().count();
			long long end = std::chrono::time_point_cast<std::chrono::microseconds>(endTimepoint).time_since_epoch().count();

			Instrumentor::Get().WriteProfile({ m_Name, start, end, std::this_thread::get_id() });

			m_Stopped = true;
		}
	private:
		const char* m_Name;
		std::chrono::time_point<std::chrono::high_resolution_clock> m_StartTimepoint;
		bool m_Stopped;
	};
}


#define RTY_PROFILE 1
#if RTY_PROFILE
#if defined(__GNUC__) || (defined(__MWERKS__) && (__MWERKS__ >= 0x3000)) || (defined(__ICC) && (__ICC >= 600)) || defined(__ghs__)
#define RTY_FUNC_SIG __PRETTY_FUNCTION__
#elif defined(__DMC__) && (__DMC__ >= 0x810)
#define RTY_FUNC_SIG __PRETTY_FUNCTION__
#elif defined(__FUNCSIG__)
#define RTY_FUNC_SIG __FUNCSIG__
#elif (defined(__INTEL_COMPILER) && (__INTEL_COMPILER >= 600)) || (defined(__IBMCPP__) && (__IBMCPP__ >= 500))
#define RTY_FUNC_SIG __FUNCTION__
#elif defined(__BORLANDC__) && (__BORLANDC__ >= 0x550)
#define RTY_FUNC_SIG __FUNC__
#elif defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 199901)
#define RTY_FUNC_SIG __func__
#elif defined(__cplusplus) && (__cplusplus >= 201103)
#define RTY_FUNC_SIG __func__
#else
#define RTY_FUNC_SIG "RTY_FUNC_SIG unknown!"
#endif

#define RTY_PROFILE_BEGIN_SESSION(name, filepath) ::raytracy::Instrumentor::Get().BeginSession(name, filepath)
#define RTY_PROFILE_END_SESSION() ::raytracy::Instrumentor::Get().EndSession()
#define RTY_PROFILE_SCOPE(name) ::raytracy::InstrumentationTimer timer##__LINE__(name);
#define RTY_PROFILE_FUNCTION() RTY_PROFILE_SCOPE(RTY_FUNC_SIG)
#else
#define RTY_PROFILE_BEGIN_SESSION(name, filepath)
#define RTY_PROFILE_END_SESSION()
#define RTY_PROFILE_SCOPE(name)
#define RTY_PROFILE_FUNCTION()
#endif