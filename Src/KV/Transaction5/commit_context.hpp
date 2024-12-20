#pragma once

#include <atomic>
#include <functional>
#include <memory>

#include "types.hpp"

namespace opossum {

/**
 * Data structure that ensures transaction are committed in an orderly manner.
 * Its main purpose is to manage commit ids.
 * It is effectively part of the TransactionContext
 *
 * Should not be used outside the concurrency module!
 */
class CommitContext {
 public:
  explicit CommitContext(const CommitID commit_id);

  CommitContext(const CommitContext& rhs) = delete;
  CommitContext& operator=(const CommitContext& rhs) = delete;

  ~CommitContext();

  CommitID commit_id() const;

  bool is_pending() const;

  /**
   * Marks the commit context as “pending”, i.e. ready to be committed
   * as soon as all previous pending have been committed.
   *
   * @param callback called when transaction is committed
   */
  void make_pending(const TransactionID transaction_id, std::function<void(TransactionID)> callback = nullptr);

  /**
   * Calls the callback of make_pending
   */
  void fire_callback();

  bool has_next() const;

  std::shared_ptr<CommitContext> next();
  std::shared_ptr<const CommitContext> next() const;

  /**
   * constructs the next context with cid + 1 or
   * returns already existing next commit context
   */
  std::shared_ptr<CommitContext> get_or_create_next();

 private:
  const CommitID _commit_id;
  std::atomic<bool> _pending;  // true if context is waiting to be committed
  std::shared_ptr<CommitContext> _next;
  std::function<void()> _callback;
};
}  // namespace opossum
