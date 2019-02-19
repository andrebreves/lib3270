/*
 * "Software pw3270, desenvolvido com base nos códigos fontes do WC3270  e X3270
 * (Paul Mattes Paul.Mattes@usa.net), de emulação de terminal 3270 para acesso a
 * aplicativos mainframe. Registro no INPI sob o nome G3270.
 *
 * Copyright (C) <2008> <Banco do Brasil S.A.>
 *
 * Este programa é software livre. Você pode redistribuí-lo e/ou modificá-lo sob
 * os termos da GPL v.2 - Licença Pública Geral  GNU,  conforme  publicado  pela
 * Free Software Foundation.
 *
 * Este programa é distribuído na expectativa de  ser  útil,  mas  SEM  QUALQUER
 * GARANTIA; sem mesmo a garantia implícita de COMERCIALIZAÇÃO ou  de  ADEQUAÇÃO
 * A QUALQUER PROPÓSITO EM PARTICULAR. Consulte a Licença Pública Geral GNU para
 * obter mais detalhes.
 *
 * Você deve ter recebido uma cópia da Licença Pública Geral GNU junto com este
 * programa; se não, escreva para a Free Software Foundation, Inc., 51 Franklin
 * St, Fifth Floor, Boston, MA  02110-1301  USA
 *
 * Este programa está nomeado como - e possui - linhas de código.
 *
 * Contatos:
 *
 * perry.werneck@gmail.com	(Alexandre Perry de Souza Werneck)
 * erico.mendonca@gmail.com	(Erico Mascarenhas Mendonça)
 *
 */

/**
 * @file src/lib3270++/ipc/session.cc
 *
 * @brief Implements lib3270 access using IPC calls.
 *
 * @author perry.werneck@gmail.com
 *
 */

 #include "../private.h"
 #include <cstring>

 using std::string;

/*---[ Implement ]----------------------------------------------------------------------------------*/

 namespace TN3270 {

	void IPC::Session::connect(const char *url) {
		Request request(*this,"connect");
		request.push(url).call();
    }

	void IPC::Session::disconnect() {
		Request(*this,"disconnect").call();
	}

	// Wait for session state.
	void IPC::Session::waitForReady(time_t timeout) {

		int rc;

		time_t end = time(nullptr) + timeout;

		while(time(nullptr) < end) {

			debug("Running waitForReady request...");

			Request(*this,"waitForReady")
				.push(1)
				.call()
				.pop(rc);

			debug("Wait for ready returned ",rc);

			if(rc == 0)
				return;

		}

		throw std::system_error(ETIMEDOUT, std::system_category());
	}

	std::string	IPC::Session::toString(int baddr, size_t len, char lf) const {

		std::string rc;

		Request(*this,"getStringAtAddress")
			.push((uint32_t) baddr)
			.push((uint32_t) len)
			.push((uint8_t) lf)
			.call()
			.pop(rc);

		return rc;
	}

	std::string	IPC::Session::toString(int row, int col, size_t sz, char lf) const {

		std::string rc;

		Request(*this,"getStringAt")
			.push((uint32_t) row)
			.push((uint32_t) col)
			.push((uint32_t) sz)
			.push((uint8_t) lf)
			.call()
			.pop(rc);

		return rc;
	}

	ProgramMessage IPC::Session::getProgramMessage() const {

		int program_message;
		getProperty("program_message",program_message);
		return (ProgramMessage) program_message;

	}

	ConnectionState IPC::Session::getConnectionState() const {

		int cstate;
		getProperty("cstate",cstate);
		return (ConnectionState) cstate;

	}

	/// @brief Set field at current position, jumps to next writable field.
	TN3270::Session & IPC::Session::push(const char *text) {
		throw std::system_error(EINVAL, std::system_category());
	}

	TN3270::Session & IPC::Session::push(int baddr, const std::string &text) {


		return *this;
	}

	TN3270::Session & IPC::Session::push(int row, int col, const std::string &text) {


		return *this;
	}

	TN3270::Session & IPC::Session::push(const PFKey key) {


		return *this;
	}

	TN3270::Session & IPC::Session::push(const PAKey key) {


		return *this;
	}

	TN3270::Session & IPC::Session::push(const Action action) {

		return *this;
	}

	TN3270::Session & IPC::Session::pop(int baddr, std::string &text) {


		return *this;
	}

	TN3270::Session & IPC::Session::pop(int row, int col, std::string &text) {

		return *this;
	}

	TN3270::Session & IPC::Session::pop(std::string &text) {

		return *this;
	}

	/// @brief Set cursor address.
	///
	/// @param addr	Cursor address.
	void IPC::Session::setCursorPosition(unsigned short addr) {


	}

	/// @brief Set cursor position.
	///
	/// @param row	New cursor row.
	/// @param col	New cursor column.
	void IPC::Session::setCursorPosition(unsigned short row, unsigned short col) {


	}

	void IPC::Session::getProperty(const char *name, int &value) const {

		Request(*this,false,name)
			.call()
			.pop(value);

	}

	void IPC::Session::getProperty(const char *name, std::string &value) const {

		Request(*this,false,name)
			.call()
			.pop(value);

	}

	void IPC::Session::getProperty(const char *name, bool &value) const {
		throw std::system_error(ENOENT, std::system_category());
	}

	/// @brief Get lib3270 version.
	std::string IPC::Session::getVersion() const {

		string rc;
		getProperty("version",rc);
		return rc;

	}

	/// @brief Get lib3270 revision.
	std::string IPC::Session::getRevision() const {

		string rc;
		getProperty("revision",rc);
		return rc;

	}

 }


